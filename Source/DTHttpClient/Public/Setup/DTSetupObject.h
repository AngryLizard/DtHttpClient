
#pragma once

#include "CoreMinimal.h"
#include "Utils/DTIdentifiers.h"
#include "Generated/DTTypes.h"
#include "Generated/DTModels.h"
#include "DTSetupObject.generated.h"

UCLASS()
class DTHTTPCLIENT_API ADTSceneActor : public AActor
{
	GENERATED_BODY()
public:
	ADTSceneActor();

	/**
	* Scene key of this scene. Defaults to the name of this actor if empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FDTSceneReg SceneKey;

	/**
	* Name key of this scene. Defaults to the name of this actor if empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name = "";
};

UCLASS()
class DTHTTPCLIENT_API ADTSetupBaseActor : public AActor
{
	GENERATED_BODY()
public:
	ADTSetupBaseActor();

	/**
	* Name key of this scene. Defaults to the name of this actor if empty.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString Name = "";

	/**
	* Whether this entity is accessible publicly (only by owners and gamemasters).
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool IsPublic = false;

	// Ptr is used to reference this actor in the setup
	FString GetPtr() const;

	// Gather properties and return type used by this setup
	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props);
};
