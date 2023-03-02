
#pragma once

#include "CoreMinimal.h"
#include "Generated/DTModels.h"

struct FDTBaseKeyframe
{
	FDateTime Timestamp;
	float Duration;
};

template<typename ValueType>
struct FDTGenericKeyframe : public FDTBaseKeyframe
{
	ValueType Value;
	ValueType Previous;
};

template<typename ValueType>
struct FDTGenericTimeline
{
	FDTGenericKeyframe<ValueType> Default;
	TArray<FDTGenericKeyframe<ValueType>> Keyframes;

	FDTGenericTimeline(const FString& Value) {
		Default.Timestamp = FDateTime(0);
		Default.Duration = 1.0f;
		LexFromString(Default.Value, *Value);
	}
	virtual ~FDTGenericTimeline() {}

	void Push(const FDTPropertyChange& Change)
	{
		// Remove any keyframes after this timestamp
		// TODO: Handle possibility keyframes could come in out of order. Remove duplicates and update Previous.
		Keyframes = Keyframes.FilterByPredicate(
			[Timestamp = Change.Timestamp](const FDTGenericKeyframe<ValueType>& Keyframe)
			{
				return Keyframe.Timestamp < Timestamp;
			});

		FDTGenericKeyframe<ValueType> Keyframe;
		Keyframe.Timestamp = Change.Timestamp;
		Keyframe.Duration = Change.Duration;
		LexFromString(Keyframe.Value, *Change.PropVal);
		Keyframe.Previous = Keyframes.Num() > 0 ? Keyframes.Last().Value : Default.Value;
		Keyframes.Emplace(Keyframe);
	}

	const FDTGenericKeyframe<ValueType>& Peek(const FDateTime& Timestamp) const
	{
		for (int32 Index = Keyframes.Num() - 1; Index >= 0; Index--)
		{
			const FDTGenericKeyframe<ValueType>& Keyframe = Keyframes[Index];
			if (Keyframe.Timestamp <= Timestamp)
			{
				return Keyframe;
			}
		}
		return Default;
	}
};

FORCEINLINE float LerpTimestamp(const FDateTime& Timestamp, const FDateTime& Start, float Duration)
{
	if (FMath::IsNearlyZero(Duration))
	{
		return Timestamp < Start ? 0.f : 1.f;
	}
	return FMath::Clamp((Timestamp - Start).GetTotalSeconds() / Duration, 0.f, 1.f);
}