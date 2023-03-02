
#pragma once

#include "CoreMinimal.h"
#include "DTTypes.h"
#include "Game/DTGameBaseObject.h"
#include "Engine/DeveloperSettings.h"
#include "DTObjects.generated.h"

UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGameEntityObject : public UDTGameBaseObject
{
	GENERATED_BODY()
public:

    FDTGenericTimeline<FString> DisplayNameProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetDisplayName(const FDateTime& Timestamp, FString& Previous, FString& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<FString>& keyframe = DisplayNameProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

    FDTGenericTimeline<FDTEntityRef> ParentProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetParent(const FDateTime& Timestamp, FDTEntityRef& Previous, FDTEntityRef& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<FDTEntityRef>& keyframe = ParentProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

    FDTGenericTimeline<FDTSceneReg> SceneProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetScene(const FDateTime& Timestamp, FDTSceneReg& Previous, FDTSceneReg& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<FDTSceneReg>& keyframe = SceneProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

	virtual bool PushChange(const FDTPropertyChange& Change) override
	{
		if (Change.PropReg.Equals(TEXT("displayName"))) { DisplayNameProperty.Push(Change); return true; }
		if (Change.PropReg.Equals(TEXT("parent"))) { ParentProperty.Push(Change); return true; }
		if (Change.PropReg.Equals(TEXT("scene"))) { SceneProperty.Push(Change); return true; }
		return Super::PushChange(Change);
	}
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{
		return Super::PushAttributes(data);
	}
	virtual EDTType GetType() const override
    {
        return EDTType::Entity;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameEntityTick, UDTGameEntityObject*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameEntityUpdate, UDTGameEntityObject*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGameEntityComponent : public UDTGameBaseComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGameEntityObject> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameEntityTick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameEntityUpdate OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{
		GameObject = Cast<UDTGameEntityObject>(InGameObject);
		return GameObject.IsValid();
	}

	virtual void GameTick(const FDateTime& Timestamp) override
	{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }
	}

	virtual void GameUpdate() override
	{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {
            OnGameUpdate.Broadcast(GameObject.Get());
        }
	}
};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetupEntityActor : public ADTSetupBaseActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void GetEntitySetupValues(ADTSceneActor* ParentScene
        , FString& DisplayName
        , FDTEntityRefSetup& Parent
        , FDTSceneRegSetup& Scene) const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{
		Super::GatherSetupProps(ParentScene, Attrs, Props);
        FString DisplayName;
        FDTEntityRefSetup Parent;
        FDTSceneRegSetup Scene;
        GetEntitySetupValues(ParentScene, DisplayName, Parent, Scene);
        Props.Emplace(TEXT("displayName"), LexToString(DisplayName));
        Props.Emplace(TEXT("parent"), LexToString(Parent));
        Props.Emplace(TEXT("scene"), LexToString(Scene));
		return TEXT("Entity");
	}
};

UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGameWorldObject : public UDTGameEntityObject
{
	GENERATED_BODY()
public:

    FDTGenericTimeline<float> TemperatureProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetTemperature(const FDateTime& Timestamp, float& Previous, float& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<float>& keyframe = TemperatureProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

    
    FDTGenericPropertyValue<FDTLevelAsset> LevelProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        const FDTLevelAsset& GetLevel() const
        {
            return LevelProperty.Value;
        }

	virtual bool PushChange(const FDTPropertyChange& Change) override
	{
		if (Change.PropReg.Equals(TEXT("temperature"))) { TemperatureProperty.Push(Change); return true; }
		return Super::PushChange(Change);
	}
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{
		if (data.PropReg.Equals(TEXT("level"))) { LexFromString(LevelProperty.Value, *data.PropVal); return true; }
		return Super::PushAttributes(data);
	}
	virtual EDTType GetType() const override
    {
        return EDTType::World;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameWorldTick, UDTGameWorldObject*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameWorldUpdate, UDTGameWorldObject*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGameWorldComponent : public UDTGameBaseComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGameWorldObject> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameWorldTick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameWorldUpdate OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{
		GameObject = Cast<UDTGameWorldObject>(InGameObject);
		return GameObject.IsValid();
	}

	virtual void GameTick(const FDateTime& Timestamp) override
	{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }
	}

	virtual void GameUpdate() override
	{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {
            OnGameUpdate.Broadcast(GameObject.Get());
        }
	}
};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetupWorldActor : public ADTSetupEntityActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void GetWorldSetupValues(ADTSceneActor* ParentScene
        , float& Temperature
        , FDTLevelAsset& Level) const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{
		Super::GatherSetupProps(ParentScene, Attrs, Props);
        float Temperature;
        FDTLevelAsset Level;
        GetWorldSetupValues(ParentScene, Temperature, Level);
        Props.Emplace(TEXT("temperature"), LexToString(Temperature));
        Attrs.Emplace(TEXT("level"), LexToString(Level));
		return TEXT("World");
	}
};

UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGameActorObject : public UDTGameEntityObject
{
	GENERATED_BODY()
public:

    FDTGenericTimeline<FDTTransform> TransformProperty = "(0, 0, 0)";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetTransform(const FDateTime& Timestamp, FDTTransform& Previous, FDTTransform& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<FDTTransform>& keyframe = TransformProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

    FDTGenericTimeline<FString> MsgProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetMsg(const FDateTime& Timestamp, FString& Previous, FString& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<FString>& keyframe = MsgProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

    
    FDTGenericPropertyValue<FDTActorAsset> ActorProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        const FDTActorAsset& GetActor() const
        {
            return ActorProperty.Value;
        }

	virtual bool PushChange(const FDTPropertyChange& Change) override
	{
		if (Change.PropReg.Equals(TEXT("transform"))) { TransformProperty.Push(Change); return true; }
		if (Change.PropReg.Equals(TEXT("msg"))) { MsgProperty.Push(Change); return true; }
		return Super::PushChange(Change);
	}
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{
		if (data.PropReg.Equals(TEXT("actor"))) { LexFromString(ActorProperty.Value, *data.PropVal); return true; }
		return Super::PushAttributes(data);
	}
	virtual EDTType GetType() const override
    {
        return EDTType::Actor;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameActorTick, UDTGameActorObject*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameActorUpdate, UDTGameActorObject*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGameActorComponent : public UDTGameBaseComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGameActorObject> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameActorTick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameActorUpdate OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{
		GameObject = Cast<UDTGameActorObject>(InGameObject);
		return GameObject.IsValid();
	}

	virtual void GameTick(const FDateTime& Timestamp) override
	{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }
	}

	virtual void GameUpdate() override
	{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {
            OnGameUpdate.Broadcast(GameObject.Get());
        }
	}
};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetupActorActor : public ADTSetupEntityActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void GetActorSetupValues(ADTSceneActor* ParentScene
        , FDTTransform& Transform
        , FString& Msg
        , FDTActorAsset& Actor) const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{
		Super::GatherSetupProps(ParentScene, Attrs, Props);
        FDTTransform Transform;
        FString Msg;
        FDTActorAsset Actor;
        GetActorSetupValues(ParentScene, Transform, Msg, Actor);
        Props.Emplace(TEXT("transform"), LexToString(Transform));
        Props.Emplace(TEXT("msg"), LexToString(Msg));
        Attrs.Emplace(TEXT("actor"), LexToString(Actor));
		return TEXT("Actor");
	}
};

UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGameCharacterObject : public UDTGameActorObject
{
	GENERATED_BODY()
public:

    FDTGenericTimeline<float> HealthProperty = "100";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetHealth(const FDateTime& Timestamp, float& Previous, float& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<float>& keyframe = HealthProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

    FDTGenericTimeline<FDTSkeletalMeshAsset> MeshProperty = "";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetMesh(const FDateTime& Timestamp, FDTSkeletalMeshAsset& Previous, FDTSkeletalMeshAsset& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<FDTSkeletalMeshAsset>& keyframe = MeshProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

	virtual bool PushChange(const FDTPropertyChange& Change) override
	{
		if (Change.PropReg.Equals(TEXT("health"))) { HealthProperty.Push(Change); return true; }
		if (Change.PropReg.Equals(TEXT("mesh"))) { MeshProperty.Push(Change); return true; }
		return Super::PushChange(Change);
	}
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{
		return Super::PushAttributes(data);
	}
	virtual EDTType GetType() const override
    {
        return EDTType::Character;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameCharacterTick, UDTGameCharacterObject*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameCharacterUpdate, UDTGameCharacterObject*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGameCharacterComponent : public UDTGameBaseComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGameCharacterObject> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameCharacterTick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameCharacterUpdate OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{
		GameObject = Cast<UDTGameCharacterObject>(InGameObject);
		return GameObject.IsValid();
	}

	virtual void GameTick(const FDateTime& Timestamp) override
	{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }
	}

	virtual void GameUpdate() override
	{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {
            OnGameUpdate.Broadcast(GameObject.Get());
        }
	}
};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetupCharacterActor : public ADTSetupActorActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void GetCharacterSetupValues(ADTSceneActor* ParentScene
        , float& Health
        , FDTSkeletalMeshAsset& Mesh) const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{
		Super::GatherSetupProps(ParentScene, Attrs, Props);
        float Health;
        FDTSkeletalMeshAsset Mesh;
        GetCharacterSetupValues(ParentScene, Health, Mesh);
        Props.Emplace(TEXT("health"), LexToString(Health));
        Props.Emplace(TEXT("mesh"), LexToString(Mesh));
		return TEXT("Character");
	}
};

UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGameYoyoObject : public UDTGameCharacterObject
{
	GENERATED_BODY()
public:

	virtual bool PushChange(const FDTPropertyChange& Change) override
	{
		return Super::PushChange(Change);
	}
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{
		return Super::PushAttributes(data);
	}
	virtual EDTType GetType() const override
    {
        return EDTType::Yoyo;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameYoyoTick, UDTGameYoyoObject*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameYoyoUpdate, UDTGameYoyoObject*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGameYoyoComponent : public UDTGameBaseComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGameYoyoObject> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameYoyoTick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameYoyoUpdate OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{
		GameObject = Cast<UDTGameYoyoObject>(InGameObject);
		return GameObject.IsValid();
	}

	virtual void GameTick(const FDateTime& Timestamp) override
	{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }
	}

	virtual void GameUpdate() override
	{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {
            OnGameUpdate.Broadcast(GameObject.Get());
        }
	}
};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetupYoyoActor : public ADTSetupCharacterActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void GetYoyoSetupValues(ADTSceneActor* ParentScene
        , float& VentSize) const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{
		Super::GatherSetupProps(ParentScene, Attrs, Props);
        float VentSize;
        GetYoyoSetupValues(ParentScene, VentSize);
        Props.Emplace(TEXT("ventSize"), LexToString(VentSize));
		return TEXT("Yoyo");
	}
};

UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGameSpawnerObject : public UDTGameActorObject
{
	GENERATED_BODY()
public:

    FDTGenericTimeline<bool> ActiveProperty = "true";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetActive(const FDateTime& Timestamp, bool& Previous, bool& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<bool>& keyframe = ActiveProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

	virtual bool PushChange(const FDTPropertyChange& Change) override
	{
		if (Change.PropReg.Equals(TEXT("active"))) { ActiveProperty.Push(Change); return true; }
		return Super::PushChange(Change);
	}
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{
		return Super::PushAttributes(data);
	}
	virtual EDTType GetType() const override
    {
        return EDTType::Spawner;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameSpawnerTick, UDTGameSpawnerObject*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameSpawnerUpdate, UDTGameSpawnerObject*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGameSpawnerComponent : public UDTGameBaseComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGameSpawnerObject> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameSpawnerTick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameSpawnerUpdate OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{
		GameObject = Cast<UDTGameSpawnerObject>(InGameObject);
		return GameObject.IsValid();
	}

	virtual void GameTick(const FDateTime& Timestamp) override
	{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }
	}

	virtual void GameUpdate() override
	{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {
            OnGameUpdate.Broadcast(GameObject.Get());
        }
	}
};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetupSpawnerActor : public ADTSetupActorActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void GetSpawnerSetupValues(ADTSceneActor* ParentScene
        , bool& Active
        , float& Radius
        , float& Duration) const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{
		Super::GatherSetupProps(ParentScene, Attrs, Props);
        bool Active;
        float Radius;
        float Duration;
        GetSpawnerSetupValues(ParentScene, Active, Radius, Duration);
        Props.Emplace(TEXT("active"), LexToString(Active));
        Props.Emplace(TEXT("radius"), LexToString(Radius));
        Props.Emplace(TEXT("duration"), LexToString(Duration));
		return TEXT("Spawner");
	}
};

UCLASS(BlueprintType)
class DTHTTPCLIENT_API UDTGameItemObject : public UDTGameActorObject
{
	GENERATED_BODY()
public:

    FDTGenericTimeline<float> WeightProperty = "10";
    UFUNCTION(BlueprintPure, Category = "DTHttp")
        void GetWeight(const FDateTime& Timestamp, float& Previous, float& Value, float& Alpha) const
        {
            const FDTGenericKeyframe<float>& keyframe = WeightProperty.Peek(Timestamp);
            Alpha = LerpTimestamp(Timestamp, keyframe.Timestamp, keyframe.Duration);
            Previous = keyframe.Previous;
            Value = keyframe.Value;
        }

	virtual bool PushChange(const FDTPropertyChange& Change) override
	{
		if (Change.PropReg.Equals(TEXT("weight"))) { WeightProperty.Push(Change); return true; }
		return Super::PushChange(Change);
	}
	virtual bool PushAttributes(const FDTAttributeData& data) override
	{
		return Super::PushAttributes(data);
	}
	virtual EDTType GetType() const override
    {
        return EDTType::Item;
    }
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGameItemTick, UDTGameItemObject*, GameObject, const FDateTime&, Timestamp);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGameItemUpdate, UDTGameItemObject*, GameObject);

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API UDTGameItemComponent : public UDTGameBaseComponent
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly, Category = "DTHttp")
		TWeakObjectPtr<UDTGameItemObject> GameObject;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameItemTick OnGameTick;
        
	UPROPERTY(BlueprintAssignable)
        FOnGameItemUpdate OnGameUpdate;
    
	virtual bool TryAssignObject(UDTGameBaseObject* InGameObject) override
	{
		GameObject = Cast<UDTGameItemObject>(InGameObject);
		return GameObject.IsValid();
	}

	virtual void GameTick(const FDateTime& Timestamp) override
	{
        Super::GameTick(Timestamp);
        if (GameObject.IsValid())
        {
            OnGameTick.Broadcast(GameObject.Get(), Timestamp);
        }
	}

	virtual void GameUpdate() override
	{
        Super::GameUpdate();
        if (GameObject.IsValid())
        {
            OnGameUpdate.Broadcast(GameObject.Get());
        }
	}
};

UCLASS(ClassGroup = (GameObjects), Blueprintable, meta = (BlueprintSpawnableComponent))
class DTHTTPCLIENT_API ADTSetupItemActor : public ADTSetupActorActor
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "DTHttp")
    void GetItemSetupValues(ADTSceneActor* ParentScene
        , float& Weight) const;

	virtual FString GatherSetupProps(ADTSceneActor* ParentScene, TMap<FString, FString>& Attrs, TMap<FString, FString>& Props) override
	{
		Super::GatherSetupProps(ParentScene, Attrs, Props);
        float Weight;
        GetItemSetupValues(ParentScene, Weight);
        Props.Emplace(TEXT("weight"), LexToString(Weight));
		return TEXT("Item");
	}
};

FORCEINLINE TSubclassOf<UDTGameBaseObject> GetTypeObjectClass(EDTType Type)
{
    switch(Type)
    {
        case EDTType::Entity: return UDTGameEntityObject::StaticClass();
        case EDTType::World: return UDTGameWorldObject::StaticClass();
        case EDTType::Actor: return UDTGameActorObject::StaticClass();
        case EDTType::Character: return UDTGameCharacterObject::StaticClass();
        case EDTType::Yoyo: return UDTGameYoyoObject::StaticClass();
        case EDTType::Spawner: return UDTGameSpawnerObject::StaticClass();
        case EDTType::Item: return UDTGameItemObject::StaticClass();
        default: return nullptr;
    }
}
