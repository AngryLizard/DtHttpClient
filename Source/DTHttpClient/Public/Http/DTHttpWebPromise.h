
#pragma once

#include "CoreMinimal.h"
#include "DTHttpWebTypes.h"

template<typename PromiseType>
using TWebPromise = TCallPromise<FWebError, PromiseType>;

template<typename PromiseType>
using TWebPromisePtr = TSharedPtr<TWebPromise<PromiseType>>;

using TWebPromiseBasePtr = TSharedPtr<TCallPromiseBase<FWebError>>;
using FWebPromiseQueue = FCallPromiseQueue<FWebError>;
