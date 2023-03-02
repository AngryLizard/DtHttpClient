
#include "Game/DTGameBaseObject.h"
#include "Components/BillboardComponent.h"
#include "DTHttpClient.h"

bool UDTGameBaseObject::GetCustomProperty(const FString& Property, const FDateTime& Timestamp, FString& Previous, FDateTime& Start, float& Duration, FString& Value) const
{
	if (const FDTGenericTimeline<FString>* Ptr = CustomProperties.Find(Property))
	{
		const FDTGenericKeyframe<FString>& keyframe = Ptr->Peek(Timestamp);
		Previous = keyframe.Previous;
		Start = keyframe.Timestamp;
		Duration = keyframe.Duration;
		Value = keyframe.Value;
		return true;
	}
	UE_LOG(DTHttpClient, Error, TEXT("Custom property %s does not exist"), *Property);
	return false;
}

bool UDTGameBaseObject::GetCustomAttribute(const FString& Attribute, FString& Value) const
{
	if (const FString* Ptr = CustomAttributes.Find(Attribute))
	{
		Value = *Ptr;
		return true;
	}
	UE_LOG(DTHttpClient, Error, TEXT("Custom attribute %s does not exist"), *Attribute);
	return false;
}

bool UDTGameBaseObject::PushChange(const FDTPropertyChange& Change)
{
	CustomProperties.FindOrAdd(Change.PropReg, FDTGenericTimeline<FString>("")).Push(Change);
	return true;
}

bool UDTGameBaseObject::PushAttributes(const FDTAttributeData& info)
{
	FString& AttributeValue = CustomAttributes.FindOrAdd(info.PropReg, FString(""));
	LexFromString(AttributeValue, *info.PropVal);
	return true;
}

EDTType UDTGameBaseObject::GetType() const
{
	return EDTType::Invalid;
}

bool UDTGameBaseComponent::TryAssignObject(UDTGameBaseObject* GameObject)
{
	return false;
}

void UDTGameBaseComponent::GameTick(const FDateTime& Timestamp)
{
}

void UDTGameBaseComponent::GameUpdate()
{
}
