// Copyright Epic Games, Inc. All Rights Reserved.

#include "DTHttpServer.h"
#include "DTHttpServerSubsystem.h"

DEFINE_LOG_CATEGORY(DTHttpServer);

#define LOCTEXT_NAMESPACE "FDTHttpServerModule"

void FDTHttpServerModule::StartupModule()
{
	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FDTHttpServerModule::RegisterMenus));
}

void FDTHttpServerModule::ShutdownModule()
{
	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

FReply FDTHttpServerModule::ServerButtonClicked()
{
	UDTHttpServerSubsystem* ServerSubsystem = UDTHttpServerSubsystem::Get();
	check(ServerSubsystem);

	const EDTHttpServerStatus Status = ServerSubsystem->GetStatus();
	if (Status == EDTHttpServerStatus::Stopped)
	{
		ServerSubsystem->RunServer();
	}

	if (Status == EDTHttpServerStatus::Running)
	{
		ServerSubsystem->StopServer();
	}
	return FReply::Handled();
}

FReply FDTHttpServerModule::SetupButtonClicked()
{
	UDTHttpServerSubsystem* ServerSubsystem = UDTHttpServerSubsystem::Get();
	check(ServerSubsystem);

	ServerSubsystem->BuildSetup();
	return FReply::Handled();
}

void FDTHttpServerModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			{
				FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");

				TSharedRef<SWidget> StatusWidget = 
					SNew(SButton)
					.OnClicked(FOnClicked::CreateRaw(this, &FDTHttpServerModule::ServerButtonClicked))
					.IsEnabled_Raw(this, &FDTHttpServerModule::IsButtonEnabled)
					.Content()
					[
						SNew(SImage)
						.Image_Raw(this, &FDTHttpServerModule::GetStatusImage)
					];

				Section.AddEntry(
					FToolMenuEntry::InitWidget(
						"RunServer",
						StatusWidget,
						FText::GetEmpty(),
						true,
						false
					));
			}

			{
				FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");

				TSharedRef<SWidget> SetupWidget =
					SNew(SButton)
					.OnClicked(FOnClicked::CreateRaw(this, &FDTHttpServerModule::SetupButtonClicked))
					.Content()
					[
						SNew(SImage)
						.Image(FAppStyle::Get().GetBrush("PlayWorld.EjectFromPlayer"))
					];

				Section.AddEntry(
					FToolMenuEntry::InitWidget(
						"BuildSetup",
						SetupWidget,
						FText::GetEmpty(),
						true,
						false
					));
			}
		}
	}
}

const FSlateBrush* FDTHttpServerModule::GetStatusImage() const
{
	UDTHttpServerSubsystem* ServerSubsystem = UDTHttpServerSubsystem::Get();
	check(ServerSubsystem);

	const EDTHttpServerStatus Status = ServerSubsystem->GetStatus();
	switch (Status)
	{
	case EDTHttpServerStatus::Stopped: return FAppStyle::Get().GetBrush("PlayWorld.ResumePlaySession");
	case EDTHttpServerStatus::Running: return FAppStyle::Get().GetBrush("PlayWorld.StopPlaySession");
	case EDTHttpServerStatus::Loading: return FAppStyle::Get().GetBrush("PlayWorld.ResumePlaySession");
	default: return FAppStyle::Get().GetBrush("Cascade.DeleteLOD");
	}
}

FText FDTHttpServerModule::GetStatusText() const
{
	UDTHttpServerSubsystem* ServerSubsystem = UDTHttpServerSubsystem::Get();
	check(ServerSubsystem);

	const EDTHttpServerStatus Status = ServerSubsystem->GetStatus();
	switch (Status)
	{
	case EDTHttpServerStatus::Stopped: return LOCTEXT("DTHttpServer_Stopped", "Not Running");
	case EDTHttpServerStatus::Running: return LOCTEXT("DTHttpServer_Running", "Server Ready!");
	case EDTHttpServerStatus::Loading: return LOCTEXT("DTHttpServer_Loading", "Loading...");
	default: return LOCTEXT("DTHttpServer_Error", "Error");
	}
}

bool FDTHttpServerModule::IsButtonEnabled() const
{
	UDTHttpServerSubsystem* ServerSubsystem = UDTHttpServerSubsystem::Get();
	check(ServerSubsystem);

	const EDTHttpServerStatus Status = ServerSubsystem->GetStatus();
	return Status != EDTHttpServerStatus::Loading;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FDTHttpServerModule, DTHttpServer)