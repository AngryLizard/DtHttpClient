
#pragma once

#include "CoreMinimal.h"

#include "DTHttpContext.generated.h"

USTRUCT()
struct DTHTTPCLIENT_API FDTHttpContext
{
    GENERATED_USTRUCT_BODY()

    UPROPERTY()
        FString Token = "";

    UPROPERTY()
        FString BaseUrl = "localhost:3000";
};