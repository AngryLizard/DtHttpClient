
#include "Setup/DTSetupObject.h"
#include "Components/BillboardComponent.h"
#include "DTHttpClient.h"

FString ADTSetupBaseActor::GetPtr() const
{
	const FString GuidString = GetActorGuid().ToString();
	const FString Ptr = Name.IsEmpty() ? GetName() : Name;
	return FString::Printf(TEXT("%s_%s"), *Ptr, *GuidString);
}

ADTSceneActor::ADTSceneActor()
	: Super()
{
	SetActorHiddenInGame(true);
	bIsEditorOnlyActor = true;
}

ADTSetupBaseActor::ADTSetupBaseActor()
	: Super()
{
	SetActorHiddenInGame(true);
	bIsEditorOnlyActor = true;
}

FString ADTSetupBaseActor::GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props)
{
	return "";
}
