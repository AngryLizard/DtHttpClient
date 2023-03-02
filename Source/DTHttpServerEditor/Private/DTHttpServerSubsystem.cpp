
#include "DTHttpServerSubsystem.h"
#include "DesktopPlatformModule.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/FileHelper.h"
#include "Dom/JsonObject.h"
#include "TimerManager.h"
#include "EngineUtils.h"
#include "Editor.h"

#include "Game/DTGameBaseObject.h"
#include "DTHttpClientSettings.h"
#include "DTHttpServer.h"

UDTHttpServerSubsystem* UDTHttpServerSubsystem::Get()
{
	check(GEditor);
	return GEditor->GetEditorSubsystem<UDTHttpServerSubsystem>();
}

bool UDTHttpServerSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	return true;
}

void UDTHttpServerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

}

void UDTHttpServerSubsystem::Deinitialize()
{
	// Stop in case server is still running
	StopServer();

	Super::Deinitialize();
}

EDTHttpServerStatus UDTHttpServerSubsystem::GetStatus() const
{
	return CurrentStatus;
}

void UDTHttpServerSubsystem::RunServer()
{
	if (IsRunning())
	{
		UE_LOG(DTHttpServer, Error, TEXT("Server is already running!"));
		return;
	}

	// Update setup before starting server
	BuildSetup();
	
	// Start server process
	static FString Content = IPluginManager::Get().FindPlugin(TEXT("DTHttp"))->GetBaseDir();
	const FString ServerPath = Content / TEXT("NodeJS/Server/build/server.js");
	const FString SetupPath = Content / TEXT("Build/Setup.json");
	const int32 Port = GetDefault<UDTHttpClientSettings>()->ServerPort;

	const FString Command = FString::Printf(TEXT("%s %s -p %d"), *ServerPath, *FPaths::ConvertRelativePathToFull(SetupPath), Port);
	UE_LOG(DTHttpServer, Log, TEXT("Trying to run 'node %s'"), *Command);

	FPlatformProcess::CreatePipe(CurrentProcessReadPipe, CurrentProcessWritePipe);
	CurrentProcessHandle = FPlatformProcess::CreateProc(TEXT("node"), *Command, false, false, false, &CurrentProcessID, 0, nullptr, CurrentProcessWritePipe, nullptr);
	if (CurrentProcessHandle.IsValid())
	{
		FPlatformProcess::Sleep(0.01);
		UE_LOG(DTHttpServer, Log, TEXT("Started NodeJS server"));
		
		GEditor->GetTimerManager()->SetTimer(CurrentPollingHandle, FTimerDelegate::CreateUObject(this, &UDTHttpServerSubsystem::OnServerPoll), 0.5f, true, 1.0f);

		CurrentStatus = EDTHttpServerStatus::Loading;
	}
}

void UDTHttpServerSubsystem::StopServer()
{
	if (IsRunning())
	{
		FPlatformProcess::TerminateProc(CurrentProcessHandle);
		UE_LOG(DTHttpServer, Log, TEXT("Stopped NodeJS server"));
	}

	CurrentStatus = EDTHttpServerStatus::Stopped;

	// Always stop polling just to be safe
	GEditor->GetTimerManager()->ClearTimer(CurrentPollingHandle);
}

void UDTHttpServerSubsystem::OnServerPoll()
{
	if (IsRunning())
	{
		FString Output = FPlatformProcess::ReadPipe(CurrentProcessReadPipe);
		if (!Output.IsEmpty())
		{
			TArray<FString> Lines;
			Output.ParseIntoArray(Lines, TEXT("\n"), true);

			for (const FString& Line : Lines)
			{
				if (Line.Contains("api started!"))
				{
					CurrentStatus = EDTHttpServerStatus::Running;
				}
				UE_LOG(DTHttpServer, Log, TEXT("Server: %s"), *Line);
			}
		}
	}
	else
	{
		UE_LOG(DTHttpServer, Warning, TEXT("NodeJS server stopped on its own"));
		StopServer();
	}
}

bool UDTHttpServerSubsystem::IsRunning()
{
	return CurrentProcessHandle.IsValid() && FPlatformProcess::IsProcRunning(CurrentProcessHandle);
}


void UDTHttpServerSubsystem::BuildSetup()
{
	FEditorScriptExecutionGuard Guard;
	UE_LOG(DTHttpServer, Log, TEXT("Building Setup file"));

	static FString Content = IPluginManager::Get().FindPlugin(TEXT("DTHttp"))->GetBaseDir();
	const FString ServerSetupPath = Content / TEXT("NodeJS/Server/build/Setup.json");
	const FString SetupPath = Content / TEXT("Build/Setup.json");

	SetupRelations Relations;
	TArray<ADTSetupBaseActor*> RootSetupObjects;

	UWorld* EditorWorld = GEditor->GetEditorWorldContext().World();
	if (!IsValid(EditorWorld))
	{
		UE_LOG(DTHttpServer, Error, TEXT("Building failed, editor world is invalid"));
		return;
	}

	// Gather setup objects
	for (TActorIterator<ADTSetupBaseActor> It(EditorWorld); It; ++It)
	{
		ADTSetupBaseActor* SetupObject = *It;
		ADTSceneActor* SceneParent = Cast<ADTSceneActor>(SetupObject->GetRootComponent()->GetAttachParentActor());
		if (IsValid(SceneParent))
		{
			Relations.Objects.FindOrAdd(SceneParent, TArray<ADTSetupBaseActor*>()).Emplace(SetupObject);
		}
		else
		{
			RootSetupObjects.Emplace(SetupObject);
		}
	}

	// Gather setup scenes
	for (TActorIterator<ADTSceneActor> It(EditorWorld); It; ++It)
	{
		ADTSceneActor* SetupScene = *It;
		ADTSetupBaseActor* ObjectParent = Cast<ADTSetupBaseActor>(SetupScene->GetRootComponent()->GetAttachParentActor());
		if (IsValid(ObjectParent))
		{
			Relations.Scenes.FindOrAdd(ObjectParent, TArray<ADTSceneActor*>()).Emplace(SetupScene);
		}
		else
		{
			UE_LOG(DTHttpServer, Error, TEXT("Root scene is not supported: %s"), *SetupScene->GetName());
		}
	}

	// Recursively build setup
	TSharedRef<FJsonObject> SetupRoot = MakeShareable(new FJsonObject);
	TArray<TSharedPtr<FJsonValue>> RootSetupJsons;
	GenerateObjectSetup(nullptr, Relations, RootSetupObjects, RootSetupJsons);
	SetupRoot->SetArrayField(TEXT("entities"), RootSetupJsons);

	FString JsonString;
	if (!FJsonSerializer::Serialize(SetupRoot, TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		UE_LOG(DTHttpServer, Error, TEXT("Json serialization failed!"));
		return;
	}

	if (!FFileHelper::SaveStringToFile(JsonString, *SetupPath))
	{
		UE_LOG(DTHttpServer, Error, TEXT("Writing json setup to file failed!"));
		return;
	}

	UE_LOG(DTHttpServer, Log, TEXT("Written setup file to %s"), *SetupPath);

	if (!FFileHelper::SaveStringToFile(JsonString, *ServerSetupPath))
	{
		UE_LOG(DTHttpServer, Error, TEXT("Writing server json setup to file failed!"));
		return;
	}

	UE_LOG(DTHttpServer, Log, TEXT("Written server setup file to %s"), *ServerSetupPath);
}

void UDTHttpServerSubsystem::GenerateObjectSetup(ADTSceneActor* ParentScene, const UDTHttpServerSubsystem::SetupRelations& Relations, const TArray<ADTSetupBaseActor*>& SetupObjects, TArray<TSharedPtr<FJsonValue>>& SetupJsons)
{
	for (ADTSetupBaseActor* SetupObject : SetupObjects)
	{
		TSharedRef<FJsonObject> SetupJson = MakeShareable(new FJsonObject);

		// Write attribute fields
		SetupJson->SetStringField(TEXT("ptr"), SetupObject->GetPtr());
		SetupJson->SetStringField(TEXT("name"), SetupObject->Name.IsEmpty() ? SetupObject->GetName() : SetupObject->Name);
		SetupJson->SetBoolField(TEXT("isPublic"), SetupObject->IsPublic);

		// Write properties
		TMap<FString, FString> Props;
		TMap<FString, FString> Attrs;
		const FString& TypeKey = SetupObject->GatherSetupProps(ParentScene, Attrs, Props);
		if (Props.Num() > 0)
		{
			TArray<TSharedPtr<FJsonValue>> PropJsons;
			for (const auto& Prop : Props)
			{
				if (Prop.Value.IsEmpty())
				{
					continue;
				}
				TSharedRef<FJsonObject> PropJson = MakeShareable(new FJsonObject);
				PropJson->SetStringField(TEXT("propKey"), Prop.Key);
				PropJson->SetStringField(TEXT("propVal"), Prop.Value);
				PropJsons.Emplace(MakeShareable(new FJsonValueObject(PropJson)));
			}
			SetupJson->SetArrayField(TEXT("propSetups"), PropJsons);

			TArray<TSharedPtr<FJsonValue>> AttrJsons;
			for (const auto& Attr : Attrs)
			{
				if (Attr.Value.IsEmpty())
				{
					continue;
				}
				TSharedRef<FJsonObject> AttrJson = MakeShareable(new FJsonObject);
				AttrJson->SetStringField(TEXT("propKey"), Attr.Key);
				AttrJson->SetStringField(TEXT("propVal"), Attr.Value);
				AttrJsons.Emplace(MakeShareable(new FJsonValueObject(AttrJson)));
			}
			SetupJson->SetArrayField(TEXT("attrSetups"), AttrJsons);
		}
		SetupJson->SetStringField(TEXT("typeKey"), TypeKey);

		// Write children
		if (const TArray<ADTSceneActor*>* Ptr = Relations.Scenes.Find(SetupObject))
		{
			TArray<TSharedPtr<FJsonValue>> ChildJsons;
			GenerateSceneSetup(SetupObject, Relations, *Ptr, ChildJsons);
			SetupJson->SetArrayField(TEXT("sceneSetups"), ChildJsons);
		}

		SetupJsons.Emplace(MakeShareable(new FJsonValueObject(SetupJson)));
	}
}

void UDTHttpServerSubsystem::GenerateSceneSetup(ADTSetupBaseActor* ParentObject, const UDTHttpServerSubsystem::SetupRelations& Relations, const TArray<ADTSceneActor*>& SetupScenes, TArray<TSharedPtr<FJsonValue>>& SetupJsons)
{
	for (ADTSceneActor* SetupScene : SetupScenes)
	{
		TSharedRef<FJsonObject> SetupJson = MakeShareable(new FJsonObject);

		// Write attribute fields
		SetupJson->SetStringField(TEXT("name"), SetupScene->Name.IsEmpty() ? SetupScene->GetName() : SetupScene->Name);
		SetupJson->SetStringField(TEXT("sceneKey"), SetupScene->SceneKey.Value.IsEmpty() ? SetupScene->GetName() : SetupScene->SceneKey.Value);

		// Write children
		if (const TArray<ADTSetupBaseActor*>* Ptr = Relations.Objects.Find(SetupScene))
		{
			TArray<TSharedPtr<FJsonValue>> ChildJsons;
			GenerateObjectSetup(SetupScene, Relations, *Ptr, ChildJsons);
			SetupJson->SetArrayField(TEXT("entitySetups"), ChildJsons);
		}

		SetupJsons.Emplace(MakeShareable(new FJsonValueObject(SetupJson)));
	}
}
