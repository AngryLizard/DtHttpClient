
#pragma once

#include "CoreMinimal.h"
#include "Game/DTGameProperties.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "DTConversions.generated.h"

UCLASS()
class DTHTTPCLIENT_API UDTGamePropertyLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (Boolean)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_BooleanToPropertyValue(bool Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::Boolean;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (Number)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_NumberToPropertyValue(float Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::Number;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (String)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_StringToPropertyValue(const FString& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::String;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (SceneReg)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_SceneRegToPropertyValue(const FDTSceneReg& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::SceneReg;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (EntityRef)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_EntityRefToPropertyValue(const FDTEntityRef& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::EntityRef;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (LevelAsset)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_LevelAssetToPropertyValue(const FDTLevelAsset& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::LevelAsset;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (ActorAsset)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_ActorAssetToPropertyValue(const FDTActorAsset& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::ActorAsset;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (StaticMeshAsset)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_StaticMeshAssetToPropertyValue(const FDTStaticMeshAsset& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::StaticMeshAsset;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (SkeletalMeshAsset)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_SkeletalMeshAssetToPropertyValue(const FDTSkeletalMeshAsset& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::SkeletalMeshAsset;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (TextureAsset)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_TextureAssetToPropertyValue(const FDTTextureAsset& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::TextureAsset;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (AnimationAsset)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_AnimationAssetToPropertyValue(const FDTAnimationAsset& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::AnimationAsset;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (Transform)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_TransformToPropertyValue(const FDTTransform& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::Transform;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (BooleanArray)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_BooleanArrayToPropertyValue(const TArray<bool>& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::BooleanArray;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (NumberArray)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_NumberArrayToPropertyValue(const TArray<float>& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::NumberArray;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (StringArray)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_StringArrayToPropertyValue(const TArray<FString>& Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::StringArray;
            Param.Value = LexToString(Value);
            return Param;
        }

	UFUNCTION(BlueprintPure, meta = (DisplayName = "To PropertyValue (Type)", CompactNodeTitle = "->", Keywords = "cast convert", BlueprintAutocast))
		static FSerialPropertyValue Conv_TypeToPropertyValue(EDTType Value)
        {
            FSerialPropertyValue Param;
            Param.Type = EDTPropertyType::Type;
            Param.Value = LexToString(Value);
            return Param;
        }

};
