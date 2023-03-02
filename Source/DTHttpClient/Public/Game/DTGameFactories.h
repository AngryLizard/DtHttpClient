
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Utils/DTIdentifiers.h"
#include "DTGameFactories.generated.h"

class UDTGameBaseComponent;
class UDTGameBaseObject;

UCLASS()
class DTHTTPCLIENT_API UDTGameFactoryAsset : public UDataAsset
{
	GENERATED_BODY()
public:

	virtual UDTGameBaseComponent* Build(UWorld* World, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject);
	virtual void Update(UDTGameBaseComponent* GameComponent, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject);
};

UCLASS(Blueprintable)
class DTHTTPCLIENT_API UDTWorldFactoryAsset : public UDTGameFactoryAsset
{
	GENERATED_BODY()
public:

	virtual UDTGameBaseComponent* Build(UWorld* World, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject) override;
	virtual void Update(UDTGameBaseComponent* GameComponent, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject) override;
};

UCLASS(Blueprintable)
class DTHTTPCLIENT_API UDTActorFactoryAsset : public UDTGameFactoryAsset
{
	GENERATED_BODY()
public:

	virtual UDTGameBaseComponent* Build(UWorld* World, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject) override;
	virtual void Update(UDTGameBaseComponent* GameComponent, const FDTEntityRef& EntityRef, UDTGameBaseObject* GameObject) override;
};