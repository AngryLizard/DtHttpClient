
#pragma once

#include "EditorSubsystem.h"
#include "DTHttpServerSubsystem.generated.h"

class ADTSetupBaseActor;
class ADTSceneActor;

UENUM()
enum class EDTHttpServerStatus : uint8
{
	Stopped,
	Loading,
	Running,
	Error
};

UCLASS()
class DTHTTPSERVEREDITOR_API UDTHttpServerSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()

public:
	static UDTHttpServerSubsystem* Get();

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	EDTHttpServerStatus GetStatus() const;
	void RunServer();
	void StopServer();

	void OnServerPoll();

private:
	bool IsRunning();

	EDTHttpServerStatus CurrentStatus = EDTHttpServerStatus::Stopped;

	void* CurrentProcessReadPipe = nullptr;
	void* CurrentProcessWritePipe = nullptr;
	FTimerHandle CurrentPollingHandle;

	FProcHandle CurrentProcessHandle;
	uint32 CurrentProcessID = 0;

public:
	void BuildSetup();

private:
	struct SetupRelations 
	{
		TMap<ADTSetupBaseActor*, TArray<ADTSceneActor*>> Scenes;
		TMap<ADTSceneActor*, TArray<ADTSetupBaseActor*>> Objects;
	};
	
	void GenerateObjectSetup(ADTSceneActor* ParentScene, const SetupRelations& Relations, const TArray<ADTSetupBaseActor*>& SetupObjects, TArray<TSharedPtr<FJsonValue>>& SetupJsons);
	void GenerateSceneSetup(ADTSetupBaseActor* ParentObject, const SetupRelations& Relations, const TArray<ADTSceneActor*>& SetupScenes, TArray<TSharedPtr<FJsonValue>>& SetupJsons);

};