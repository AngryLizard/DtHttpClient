
#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"

#include "DTModels.generated.h"


/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTTypePropData
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString PropReg;

    /**  */
    UPROPERTY()
        FString Desc;

    /**  */
    UPROPERTY()
        FString Type;

    /**  */
    UPROPERTY()
        FString Initial;
};

FORCEINLINE FString LexToString(const FDTTypePropData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTTypePropData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTTypePropData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTTypePropData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTTypePropData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTTypePropData>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTTypeData
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString TypeReg;

    /**  */
    UPROPERTY()
        FString Asset;

    /**  */
    UPROPERTY()
        FString Parent;

    /**  */
    UPROPERTY()
        FString Desc;

    /**  */
    UPROPERTY()
        TArray<FDTTypePropData> Props;

    /**  */
    UPROPERTY()
        TArray<FDTTypePropData> Attrs;
};

FORCEINLINE FString LexToString(const FDTTypeData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTTypeData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTTypeData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTTypeData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTTypeData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTTypeData>(Buffer, &Value);
};

/** Hypertext Transfer Protocol (HTTP) response status codes. */
UENUM()
enum class EDTHttpStatusCode : uint8
{
    None,
    Code100,
    Code101,
    Code102,
    Code200,
    Code201,
    Code202,
    Code203,
    Code204,
    Code205,
    Code206,
    Code207,
    Code208,
    Code226,
    Code300,
    Code301,
    Code302,
    Code303,
    Code304,
    Code305,
    Code306,
    Code307,
    Code308,
    Code400,
    Code401,
    Code402,
    Code403,
    Code404,
    Code405,
    Code406,
    Code407,
    Code408,
    Code409,
    Code410,
    Code411,
    Code412,
    Code413,
    Code414,
    Code415,
    Code416,
    Code417,
    Code418,
    Code421,
    Code422,
    Code423,
    Code424,
    Code426,
    Code428,
    Code429,
    Code431,
    Code451,
    Code500,
    Code501,
    Code502,
    Code503,
    Code504,
    Code505,
    Code506,
    Code507,
    Code508,
    Code510,
    Code511,
};

FORCEINLINE FString LexToString(EDTHttpStatusCode Value)
{
    switch (Value)
    {
    case EDTHttpStatusCode::Code100: return TEXT("100");
    case EDTHttpStatusCode::Code101: return TEXT("101");
    case EDTHttpStatusCode::Code102: return TEXT("102");
    case EDTHttpStatusCode::Code200: return TEXT("200");
    case EDTHttpStatusCode::Code201: return TEXT("201");
    case EDTHttpStatusCode::Code202: return TEXT("202");
    case EDTHttpStatusCode::Code203: return TEXT("203");
    case EDTHttpStatusCode::Code204: return TEXT("204");
    case EDTHttpStatusCode::Code205: return TEXT("205");
    case EDTHttpStatusCode::Code206: return TEXT("206");
    case EDTHttpStatusCode::Code207: return TEXT("207");
    case EDTHttpStatusCode::Code208: return TEXT("208");
    case EDTHttpStatusCode::Code226: return TEXT("226");
    case EDTHttpStatusCode::Code300: return TEXT("300");
    case EDTHttpStatusCode::Code301: return TEXT("301");
    case EDTHttpStatusCode::Code302: return TEXT("302");
    case EDTHttpStatusCode::Code303: return TEXT("303");
    case EDTHttpStatusCode::Code304: return TEXT("304");
    case EDTHttpStatusCode::Code305: return TEXT("305");
    case EDTHttpStatusCode::Code306: return TEXT("306");
    case EDTHttpStatusCode::Code307: return TEXT("307");
    case EDTHttpStatusCode::Code308: return TEXT("308");
    case EDTHttpStatusCode::Code400: return TEXT("400");
    case EDTHttpStatusCode::Code401: return TEXT("401");
    case EDTHttpStatusCode::Code402: return TEXT("402");
    case EDTHttpStatusCode::Code403: return TEXT("403");
    case EDTHttpStatusCode::Code404: return TEXT("404");
    case EDTHttpStatusCode::Code405: return TEXT("405");
    case EDTHttpStatusCode::Code406: return TEXT("406");
    case EDTHttpStatusCode::Code407: return TEXT("407");
    case EDTHttpStatusCode::Code408: return TEXT("408");
    case EDTHttpStatusCode::Code409: return TEXT("409");
    case EDTHttpStatusCode::Code410: return TEXT("410");
    case EDTHttpStatusCode::Code411: return TEXT("411");
    case EDTHttpStatusCode::Code412: return TEXT("412");
    case EDTHttpStatusCode::Code413: return TEXT("413");
    case EDTHttpStatusCode::Code414: return TEXT("414");
    case EDTHttpStatusCode::Code415: return TEXT("415");
    case EDTHttpStatusCode::Code416: return TEXT("416");
    case EDTHttpStatusCode::Code417: return TEXT("417");
    case EDTHttpStatusCode::Code418: return TEXT("418");
    case EDTHttpStatusCode::Code421: return TEXT("421");
    case EDTHttpStatusCode::Code422: return TEXT("422");
    case EDTHttpStatusCode::Code423: return TEXT("423");
    case EDTHttpStatusCode::Code424: return TEXT("424");
    case EDTHttpStatusCode::Code426: return TEXT("426");
    case EDTHttpStatusCode::Code428: return TEXT("428");
    case EDTHttpStatusCode::Code429: return TEXT("429");
    case EDTHttpStatusCode::Code431: return TEXT("431");
    case EDTHttpStatusCode::Code451: return TEXT("451");
    case EDTHttpStatusCode::Code500: return TEXT("500");
    case EDTHttpStatusCode::Code501: return TEXT("501");
    case EDTHttpStatusCode::Code502: return TEXT("502");
    case EDTHttpStatusCode::Code503: return TEXT("503");
    case EDTHttpStatusCode::Code504: return TEXT("504");
    case EDTHttpStatusCode::Code505: return TEXT("505");
    case EDTHttpStatusCode::Code506: return TEXT("506");
    case EDTHttpStatusCode::Code507: return TEXT("507");
    case EDTHttpStatusCode::Code508: return TEXT("508");
    case EDTHttpStatusCode::Code510: return TEXT("510");
    case EDTHttpStatusCode::Code511: return TEXT("511");
    default: return TEXT("");
    }
};

FORCEINLINE void LexFromString(EDTHttpStatusCode& Value, const TCHAR* Buffer)
{
    FString Name = FString(Buffer);
    if (Name.Equals(TEXT("100"))) Value = EDTHttpStatusCode::Code100; else
    if (Name.Equals(TEXT("101"))) Value = EDTHttpStatusCode::Code101; else
    if (Name.Equals(TEXT("102"))) Value = EDTHttpStatusCode::Code102; else
    if (Name.Equals(TEXT("200"))) Value = EDTHttpStatusCode::Code200; else
    if (Name.Equals(TEXT("201"))) Value = EDTHttpStatusCode::Code201; else
    if (Name.Equals(TEXT("202"))) Value = EDTHttpStatusCode::Code202; else
    if (Name.Equals(TEXT("203"))) Value = EDTHttpStatusCode::Code203; else
    if (Name.Equals(TEXT("204"))) Value = EDTHttpStatusCode::Code204; else
    if (Name.Equals(TEXT("205"))) Value = EDTHttpStatusCode::Code205; else
    if (Name.Equals(TEXT("206"))) Value = EDTHttpStatusCode::Code206; else
    if (Name.Equals(TEXT("207"))) Value = EDTHttpStatusCode::Code207; else
    if (Name.Equals(TEXT("208"))) Value = EDTHttpStatusCode::Code208; else
    if (Name.Equals(TEXT("226"))) Value = EDTHttpStatusCode::Code226; else
    if (Name.Equals(TEXT("300"))) Value = EDTHttpStatusCode::Code300; else
    if (Name.Equals(TEXT("301"))) Value = EDTHttpStatusCode::Code301; else
    if (Name.Equals(TEXT("302"))) Value = EDTHttpStatusCode::Code302; else
    if (Name.Equals(TEXT("303"))) Value = EDTHttpStatusCode::Code303; else
    if (Name.Equals(TEXT("304"))) Value = EDTHttpStatusCode::Code304; else
    if (Name.Equals(TEXT("305"))) Value = EDTHttpStatusCode::Code305; else
    if (Name.Equals(TEXT("306"))) Value = EDTHttpStatusCode::Code306; else
    if (Name.Equals(TEXT("307"))) Value = EDTHttpStatusCode::Code307; else
    if (Name.Equals(TEXT("308"))) Value = EDTHttpStatusCode::Code308; else
    if (Name.Equals(TEXT("400"))) Value = EDTHttpStatusCode::Code400; else
    if (Name.Equals(TEXT("401"))) Value = EDTHttpStatusCode::Code401; else
    if (Name.Equals(TEXT("402"))) Value = EDTHttpStatusCode::Code402; else
    if (Name.Equals(TEXT("403"))) Value = EDTHttpStatusCode::Code403; else
    if (Name.Equals(TEXT("404"))) Value = EDTHttpStatusCode::Code404; else
    if (Name.Equals(TEXT("405"))) Value = EDTHttpStatusCode::Code405; else
    if (Name.Equals(TEXT("406"))) Value = EDTHttpStatusCode::Code406; else
    if (Name.Equals(TEXT("407"))) Value = EDTHttpStatusCode::Code407; else
    if (Name.Equals(TEXT("408"))) Value = EDTHttpStatusCode::Code408; else
    if (Name.Equals(TEXT("409"))) Value = EDTHttpStatusCode::Code409; else
    if (Name.Equals(TEXT("410"))) Value = EDTHttpStatusCode::Code410; else
    if (Name.Equals(TEXT("411"))) Value = EDTHttpStatusCode::Code411; else
    if (Name.Equals(TEXT("412"))) Value = EDTHttpStatusCode::Code412; else
    if (Name.Equals(TEXT("413"))) Value = EDTHttpStatusCode::Code413; else
    if (Name.Equals(TEXT("414"))) Value = EDTHttpStatusCode::Code414; else
    if (Name.Equals(TEXT("415"))) Value = EDTHttpStatusCode::Code415; else
    if (Name.Equals(TEXT("416"))) Value = EDTHttpStatusCode::Code416; else
    if (Name.Equals(TEXT("417"))) Value = EDTHttpStatusCode::Code417; else
    if (Name.Equals(TEXT("418"))) Value = EDTHttpStatusCode::Code418; else
    if (Name.Equals(TEXT("421"))) Value = EDTHttpStatusCode::Code421; else
    if (Name.Equals(TEXT("422"))) Value = EDTHttpStatusCode::Code422; else
    if (Name.Equals(TEXT("423"))) Value = EDTHttpStatusCode::Code423; else
    if (Name.Equals(TEXT("424"))) Value = EDTHttpStatusCode::Code424; else
    if (Name.Equals(TEXT("426"))) Value = EDTHttpStatusCode::Code426; else
    if (Name.Equals(TEXT("428"))) Value = EDTHttpStatusCode::Code428; else
    if (Name.Equals(TEXT("429"))) Value = EDTHttpStatusCode::Code429; else
    if (Name.Equals(TEXT("431"))) Value = EDTHttpStatusCode::Code431; else
    if (Name.Equals(TEXT("451"))) Value = EDTHttpStatusCode::Code451; else
    if (Name.Equals(TEXT("500"))) Value = EDTHttpStatusCode::Code500; else
    if (Name.Equals(TEXT("501"))) Value = EDTHttpStatusCode::Code501; else
    if (Name.Equals(TEXT("502"))) Value = EDTHttpStatusCode::Code502; else
    if (Name.Equals(TEXT("503"))) Value = EDTHttpStatusCode::Code503; else
    if (Name.Equals(TEXT("504"))) Value = EDTHttpStatusCode::Code504; else
    if (Name.Equals(TEXT("505"))) Value = EDTHttpStatusCode::Code505; else
    if (Name.Equals(TEXT("506"))) Value = EDTHttpStatusCode::Code506; else
    if (Name.Equals(TEXT("507"))) Value = EDTHttpStatusCode::Code507; else
    if (Name.Equals(TEXT("508"))) Value = EDTHttpStatusCode::Code508; else
    if (Name.Equals(TEXT("510"))) Value = EDTHttpStatusCode::Code510; else
    if (Name.Equals(TEXT("511"))) Value = EDTHttpStatusCode::Code511; else {}
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTApiError
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString Name;

    /**  */
    UPROPERTY()
        FString Message;

    /**  */
    UPROPERTY()
        FString Stack;

    /**  */
    UPROPERTY()
        EDTHttpStatusCode Code;
};

FORCEINLINE FString LexToString(const FDTApiError& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTApiError>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTApiError& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTApiError>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTApiError>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTApiError>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTActionParamData
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString ParamReg;

    /**  */
    UPROPERTY()
        FString Input;

    /**  */
    UPROPERTY()
        FString Desc;

    /**  */
    UPROPERTY()
        FString Type;

    /**  */
    UPROPERTY()
        FString Initial;
};

FORCEINLINE FString LexToString(const FDTActionParamData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTActionParamData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTActionParamData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTActionParamData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTActionParamData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTActionParamData>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTActionData
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString ActionReg;

    /**  */
    UPROPERTY()
        FString Domain;

    /**  */
    UPROPERTY()
        FString Trigger;

    /**  */
    UPROPERTY()
        FString Desc;

    /**  */
    UPROPERTY()
        TArray<FDTActionParamData> Params;
};

FORCEINLINE FString LexToString(const FDTActionData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTActionData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTActionData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTActionData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTActionData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTActionData>(Buffer, &Value);
};

/** Static attributes stored on an entity,
these can only be changed on spawn and never again */
USTRUCT()
struct DTHTTPCLIENT_API FDTAttributeData
{
    GENERATED_USTRUCT_BODY()

    /** Property name, should correspond to an attribute in the entity type */
    UPROPERTY()
        FString PropReg;

    /** Property value whose format corresponds to property type assigned by propKey */
    UPROPERTY()
        FString PropVal;
};

FORCEINLINE FString LexToString(const FDTAttributeData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTAttributeData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTAttributeData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTAttributeData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTAttributeData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTAttributeData>(Buffer, &Value);
};

/** Generally addressable objects in the game */
USTRUCT()
struct DTHTTPCLIENT_API FDTEntityData
{
    GENERATED_USTRUCT_BODY()

    /** ID of this entity */
    UPROPERTY()
        FString Ref;

    /** Name of this entity */
    UPROPERTY()
        FString Name;

    /** Type of this entity */
    UPROPERTY()
        FString TypeReg;

    /** Owners over this entity */
    UPROPERTY()
        TArray<FString> OwnerRefs;

    /** Attributes of this entity */
    UPROPERTY()
        TArray<FDTAttributeData> Attributes;
};

FORCEINLINE FString LexToString(const FDTEntityData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTEntityData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTEntityData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTEntityData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTEntityData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTEntityData>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTEntityRequest
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString Name;

    /**  */
    UPROPERTY()
        bool IsPublic;
};

FORCEINLINE FString LexToString(const FDTEntityRequest& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTEntityRequest>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTEntityRequest& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTEntityRequest>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTEntityRequest>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTEntityRequest>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTOwnershipRequest
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        TArray<FString> PlayerRefs;
};

FORCEINLINE FString LexToString(const FDTOwnershipRequest& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTOwnershipRequest>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTOwnershipRequest& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTOwnershipRequest>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTOwnershipRequest>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTOwnershipRequest>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTSceneRequest
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString Name;
};

FORCEINLINE FString LexToString(const FDTSceneRequest& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTSceneRequest>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTSceneRequest& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTSceneRequest>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTSceneRequest>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTSceneRequest>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTTokenResult
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString Token;

    /**  */
    UPROPERTY()
        FString PlayerRef;
};

FORCEINLINE FString LexToString(const FDTTokenResult& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTTokenResult>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTTokenResult& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTTokenResult>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTTokenResult>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTTokenResult>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTPlayerCredentials
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString Username;

    /**  */
    UPROPERTY()
        FString Password;
};

FORCEINLINE FString LexToString(const FDTPlayerCredentials& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTPlayerCredentials>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTPlayerCredentials& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTPlayerCredentials>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTPlayerCredentials>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTPlayerCredentials>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTPlayerRequest
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString Username;

    /**  */
    UPROPERTY()
        FString Password;

    /**  */
    UPROPERTY()
        FString Nickname;
};

FORCEINLINE FString LexToString(const FDTPlayerRequest& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTPlayerRequest>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTPlayerRequest& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTPlayerRequest>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTPlayerRequest>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTPlayerRequest>(Buffer, &Value);
};

/** Access rights mirrored by JWToken scopes */
UENUM()
enum class EDTRole : uint8
{
    None,
    Code0,
    Code1,
    Code2,
    Code3,
    Code4,
};

FORCEINLINE FString LexToString(EDTRole Value)
{
    switch (Value)
    {
    case EDTRole::Code0: return TEXT("0");
    case EDTRole::Code1: return TEXT("1");
    case EDTRole::Code2: return TEXT("2");
    case EDTRole::Code3: return TEXT("3");
    case EDTRole::Code4: return TEXT("4");
    default: return TEXT("");
    }
};

FORCEINLINE void LexFromString(EDTRole& Value, const TCHAR* Buffer)
{
    FString Name = FString(Buffer);
    if (Name.Equals(TEXT("0"))) Value = EDTRole::Code0; else
    if (Name.Equals(TEXT("1"))) Value = EDTRole::Code1; else
    if (Name.Equals(TEXT("2"))) Value = EDTRole::Code2; else
    if (Name.Equals(TEXT("3"))) Value = EDTRole::Code3; else
    if (Name.Equals(TEXT("4"))) Value = EDTRole::Code4; else {}
};

/** Private-facing information about a player */
USTRUCT()
struct DTHTTPCLIENT_API FDTUserData
{
    GENERATED_USTRUCT_BODY()

    /** ID of this player */
    UPROPERTY()
        FString Ref;

    /** Displayed player name */
    UPROPERTY()
        FString Nickname;

    /** Displayed role */
    UPROPERTY()
        EDTRole Access;

    /** Displayed user name */
    UPROPERTY()
        FString Username;
};

FORCEINLINE FString LexToString(const FDTUserData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTUserData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTUserData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTUserData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTUserData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTUserData>(Buffer, &Value);
};

/** Public-facing information about a player */
USTRUCT()
struct DTHTTPCLIENT_API FDTPlayerData
{
    GENERATED_USTRUCT_BODY()

    /** ID of this player */
    UPROPERTY()
        FString Ref;

    /** Displayed player name */
    UPROPERTY()
        FString Nickname;

    /** Displayed role */
    UPROPERTY()
        EDTRole Access;
};

FORCEINLINE FString LexToString(const FDTPlayerData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTPlayerData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTPlayerData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTPlayerData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTPlayerData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTPlayerData>(Buffer, &Value);
};

/**  */
UENUM()
enum class EDTPermissionQuery : uint8
{
    Spectator,
    Participant,
    Gamemaster,
    Moderator,
    Admin,
};

FORCEINLINE FString LexToString(EDTPermissionQuery Value)
{
    switch (Value)
    {
    case EDTPermissionQuery::Spectator: return TEXT("spectator");
    case EDTPermissionQuery::Participant: return TEXT("participant");
    case EDTPermissionQuery::Gamemaster: return TEXT("gamemaster");
    case EDTPermissionQuery::Moderator: return TEXT("moderator");
    case EDTPermissionQuery::Admin: return TEXT("admin");
    default: return TEXT("");
    }
};

FORCEINLINE void LexFromString(EDTPermissionQuery& Value, const TCHAR* Buffer)
{
    FString Name = FString(Buffer);
    if (Name.Equals(TEXT("spectator"))) Value = EDTPermissionQuery::Spectator; else
    if (Name.Equals(TEXT("participant"))) Value = EDTPermissionQuery::Participant; else
    if (Name.Equals(TEXT("gamemaster"))) Value = EDTPermissionQuery::Gamemaster; else
    if (Name.Equals(TEXT("moderator"))) Value = EDTPermissionQuery::Moderator; else
    if (Name.Equals(TEXT("admin"))) Value = EDTPermissionQuery::Admin; else {}
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTObserveData
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        TArray<FString> EntityRefs;
};

FORCEINLINE FString LexToString(const FDTObserveData& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTObserveData>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTObserveData& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTObserveData>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTObserveData>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTObserveData>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTPropertyChange
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        FString EntityRef;

    /**  */
    UPROPERTY()
        FString PropReg;

    /**  */
    UPROPERTY()
        FDateTime Timestamp;

    /**  */
    UPROPERTY()
        float Duration;

    /**  */
    UPROPERTY()
        FString PropVal;
};

FORCEINLINE FString LexToString(const FDTPropertyChange& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTPropertyChange>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTPropertyChange& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTPropertyChange>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTPropertyChange>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTPropertyChange>(Buffer, &Value);
};

/**  */
USTRUCT()
struct DTHTTPCLIENT_API FDTSessionUpdate
{
    GENERATED_USTRUCT_BODY()

    /**  */
    UPROPERTY()
        TArray<FDTPropertyChange> Changes;

    /**  */
    UPROPERTY()
        FDateTime Timestamp;
};

FORCEINLINE FString LexToString(const FDTSessionUpdate& Value)
{
    FString Output;
    FJsonObjectConverter::UStructToJsonObjectString<FDTSessionUpdate>(Value, Output);
    return *Output;
};

FORCEINLINE void LexFromString(FDTSessionUpdate& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonObjectStringToUStruct<FDTSessionUpdate>(Buffer, &Value);
};

FORCEINLINE void LexFromString(TArray<FDTSessionUpdate>& Value, const TCHAR* Buffer)
{
    FJsonObjectConverter::JsonArrayStringToUStruct<FDTSessionUpdate>(Buffer, &Value);
};

/**  */
UENUM()
enum class EDTPropertyQuery : uint8
{
    Refresh,
    Diff,
};

FORCEINLINE FString LexToString(EDTPropertyQuery Value)
{
    switch (Value)
    {
    case EDTPropertyQuery::Refresh: return TEXT("refresh");
    case EDTPropertyQuery::Diff: return TEXT("diff");
    default: return TEXT("");
    }
};

FORCEINLINE void LexFromString(EDTPropertyQuery& Value, const TCHAR* Buffer)
{
    FString Name = FString(Buffer);
    if (Name.Equals(TEXT("refresh"))) Value = EDTPropertyQuery::Refresh; else
    if (Name.Equals(TEXT("diff"))) Value = EDTPropertyQuery::Diff; else {}
};