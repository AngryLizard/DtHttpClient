
#pragma once

#include "CoreMinimal.h"
#include "Utils/DTIdentifiers.h"
#include "Generated/DTTypes.h"
#include "Generated/DTModels.h"
#include "Game/DTGameProperties.h"
#include "Utils/DTGenericTimeline.h"
#include "DTGameBaseObject.generated.h"

UCLASS()
class DTHTTPCLIENT_API UDTGameBaseObject : public UObject
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
		FString Name = "";

	UPROPERTY(BlueprintReadOnly)
		FString TypeReg = "";

	UPROPERTY(BlueprintReadOnly)
		TArray<FString> OwnerRefs;

	TMap<FString, FDTGenericTimeline<FString>> CustomProperties;
	UFUNCTION(BlueprintPure, Category = "DTHttp")
		bool GetCustomProperty(const FString& Property, const FDateTime& Timestamp, FString& Previous, FDateTime& Start, float& Duration, FString& Value) const;

	TMap<FString, FString> CustomAttributes;
	UFUNCTION(BlueprintPure, Category = "DTHttp")
		bool GetCustomAttribute(const FString& Attribute, FString& Value) const;

	// Add a change to this object
	virtual bool PushChange(const FDTPropertyChange& Change);

	// Add an atribute to this object
	virtual bool PushAttributes(const FDTAttributeData& info);

	// Get object type
	UFUNCTION(BlueprintPure)
		virtual EDTType GetType() const;
};

UCLASS()
class DTHTTPCLIENT_API UDTGameBaseComponent : public UActorComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
		FDTEntityRef EntityRef;

	// Assign a game object to this component if compatible
	virtual bool TryAssignObject(UDTGameBaseObject* GameObject);

	// Tick this component
	virtual void GameTick(const FDateTime& Timestamp);

	// Update this component
	virtual void GameUpdate();
};
