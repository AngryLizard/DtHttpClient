
#pragma once

#include "Http/DTHttpRequester.h"
#include "Http/DTHttpContext.h"
#include "DTModels.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DTApi.generated.h"

/**
 * 
 */
UCLASS()
class DTHTTPCLIENT_API UDTApi : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
    /**  */
    static TWebPromisePtr<FResponseData<TArray<FDTTypeData>>> GetGameTypes(
        const FDTHttpContext& Context, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, TArray<FDTTypeData>>(
            Context, "GET", FString::Format(TEXT("/game/Types"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<TArray<FDTActionData>>> GetGameActions(
        const FDTHttpContext& Context, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, TArray<FDTActionData>>(
            Context, "GET", FString::Format(TEXT("/game/Actions"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<TArray<FDTEntityData>>> GetGameEntityListWith(
        const FDTHttpContext& Context, const FString& TypeReg, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("typeReg"), SanitizeArg(LexToString(TypeReg)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, TArray<FDTEntityData>>(
            Context, "GET", FString::Format(TEXT("/game/entity/list/{typeReg}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FString>> PostGameEntityWith(
        const FDTHttpContext& Context, const FString& TypeReg, 
        const FDTEntityRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("typeReg"), SanitizeArg(LexToString(TypeReg)));

        return UDTHttpRequester::SendHttpRequest<FDTEntityRequest, FString>(
            Context, "POST", FString::Format(TEXT("/game/entity/{typeReg}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FEmptyResult>> PutGameEntityWith(
        const FDTHttpContext& Context, const FString& EntityRef, 
        const FDTEntityRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRef"), SanitizeArg(LexToString(EntityRef)));

        return UDTHttpRequester::SendHttpRequest<FDTEntityRequest, FEmptyResult>(
            Context, "PUT", FString::Format(TEXT("/game/entity/{entityRef}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FDTEntityData>> GetGameEntityWith(
        const FDTHttpContext& Context, const FString& EntityRef, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRef"), SanitizeArg(LexToString(EntityRef)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FDTEntityData>(
            Context, "GET", FString::Format(TEXT("/game/entity/{entityRef}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FEmptyResult>> DeleteGameEntityWith(
        const FDTHttpContext& Context, const FString& EntityRef, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRef"), SanitizeArg(LexToString(EntityRef)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FEmptyResult>(
            Context, "DELETE", FString::Format(TEXT("/game/entity/{entityRef}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<TArray<FDTEntityData>>> GetGameListEntities(
        const FDTHttpContext& Context, const TArray<FString>& EntityRefs, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRefs"), QueryListArgs(TEXT("entityRefs"), EntityRefs));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, TArray<FDTEntityData>>(
            Context, "GET", FString::Format(TEXT("/game/list/entities?entityRefs={entityRefs}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FEmptyResult>> PutGameEntityWithOwnership(
        const FDTHttpContext& Context, const FString& EntityRef, 
        const FDTOwnershipRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRef"), SanitizeArg(LexToString(EntityRef)));

        return UDTHttpRequester::SendHttpRequest<FDTOwnershipRequest, FEmptyResult>(
            Context, "PUT", FString::Format(TEXT("/game/entity/{entityRef}/ownership"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FString>> PostGameEntityWith(
        const FDTHttpContext& Context, const FString& EntityRef, const FString& SceneReg, 
        const FDTSceneRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRef"), SanitizeArg(LexToString(EntityRef)));
        Args.Add(TEXT("sceneReg"), SanitizeArg(LexToString(SceneReg)));

        return UDTHttpRequester::SendHttpRequest<FDTSceneRequest, FString>(
            Context, "POST", FString::Format(TEXT("/game/entity/{entityRef}/scene/{sceneReg}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FEmptyResult>> DeleteGameEntityWith(
        const FDTHttpContext& Context, const FString& EntityRef, const FString& SceneReg, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRef"), SanitizeArg(LexToString(EntityRef)));
        Args.Add(TEXT("sceneReg"), SanitizeArg(LexToString(SceneReg)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FEmptyResult>(
            Context, "DELETE", FString::Format(TEXT("/game/entity/{entityRef}/scene/{sceneReg}"), Args), RequestBody);
    }

    /** Authenticate with server and return JWToken. */
    static TWebPromisePtr<FResponseData<FDTTokenResult>> PostPlayerLogin(
        const FDTHttpContext& Context, 
        const FDTPlayerCredentials& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FDTPlayerCredentials, FDTTokenResult>(
            Context, "POST", FString::Format(TEXT("/player/login"), Args), RequestBody);
    }

    /** Register yourself to this server if enabled. */
    static TWebPromisePtr<FResponseData<FString>> PostPlayerRegister(
        const FDTHttpContext& Context, 
        const FDTPlayerRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FDTPlayerRequest, FString>(
            Context, "POST", FString::Format(TEXT("/player/register"), Args), RequestBody);
    }

    /** Create new user on this server. */
    static TWebPromisePtr<FResponseData<FString>> PostPlayer(
        const FDTHttpContext& Context, 
        const FDTPlayerRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FDTPlayerRequest, FString>(
            Context, "POST", FString::Format(TEXT("/player"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FDTUserData>> GetPlayer(
        const FDTHttpContext& Context, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FDTUserData>(
            Context, "GET", FString::Format(TEXT("/player"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FEmptyResult>> PutPlayer(
        const FDTHttpContext& Context, 
        const FDTPlayerRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FDTPlayerRequest, FEmptyResult>(
            Context, "PUT", FString::Format(TEXT("/player"), Args), RequestBody);
    }

    /** Removes calling player from server. */
    static TWebPromisePtr<FResponseData<FEmptyResult>> DeletePlayer(
        const FDTHttpContext& Context, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FEmptyResult>(
            Context, "DELETE", FString::Format(TEXT("/player"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FDTPlayerData>> GetPlayerWith(
        const FDTHttpContext& Context, const FString& PlayerRef, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("playerRef"), SanitizeArg(LexToString(PlayerRef)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FDTPlayerData>(
            Context, "GET", FString::Format(TEXT("/player/{playerRef}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FEmptyResult>> PutPlayerWith(
        const FDTHttpContext& Context, const FString& PlayerRef, 
        const FDTPlayerRequest& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("playerRef"), SanitizeArg(LexToString(PlayerRef)));

        return UDTHttpRequester::SendHttpRequest<FDTPlayerRequest, FEmptyResult>(
            Context, "PUT", FString::Format(TEXT("/player/{playerRef}"), Args), RequestBody);
    }

    /** Removes a player from this server.
Users that are moderators or admins cannot be removed and must be demoted first. */
    static TWebPromisePtr<FResponseData<FEmptyResult>> DeletePlayerWith(
        const FDTHttpContext& Context, const FString& PlayerRef, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("playerRef"), SanitizeArg(LexToString(PlayerRef)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FEmptyResult>(
            Context, "DELETE", FString::Format(TEXT("/player/{playerRef}"), Args), RequestBody);
    }

    /**  */
    static TWebPromisePtr<FResponseData<FEmptyResult>> PutPlayerWith(
        const FDTHttpContext& Context, const FString& PlayerRef, const EDTPermissionQuery& Query, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("playerRef"), SanitizeArg(LexToString(PlayerRef)));
        Args.Add(TEXT("query"), SanitizeArg(LexToString(Query)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FEmptyResult>(
            Context, "PUT", FString::Format(TEXT("/player/{playerRef}/{query}"), Args), RequestBody);
    }

    /** Start a new session as currently logged in player.
After a session has been started, the player needs to query for an update every so often
(even if they don't observe any entities) or the server assumes the player has disconnected.

Note that we give players the ability to have multiple sessions active at the same time,
e.g. if they're running the game multiple times in different windows. */
    static TWebPromisePtr<FResponseData<FString>> PostSessionStart(
        const FDTHttpContext& Context, 
        const FDTObserveData& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FDTObserveData, FString>(
            Context, "POST", FString::Format(TEXT("/session/start"), Args), RequestBody);
    }

    /** Subscribe to timeline changes (SSE). Setup can take a while so this will also perform a diff on the database.
Usually a client will first do an update with refresh and then subscribe to further changes. */
    static TWebPromisePtr<FResponseData<FEmptyResult>> GetSessionEventsWith(
        const FDTHttpContext& Context, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FEmptyResult>(
            Context, "GET", FString::Format(TEXT("/session/events/{sessionReg}"), Args), RequestBody);
    }

    /** Get pending updates from an active session. Also refresh the heartbeat timer so
that the session keeps on living. */
    static TWebPromisePtr<FResponseData<TArray<FDTPropertyChange>>> PostSessionUpdateWith(
        const FDTHttpContext& Context, const FString& SessionReg, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("sessionReg"), SanitizeArg(LexToString(SessionReg)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, TArray<FDTPropertyChange>>(
            Context, "POST", FString::Format(TEXT("/session/update/{sessionReg}"), Args), RequestBody);
    }

    /** Get state from an active session and resets session queue.
This directly queries the database and discards current session state. */
    static TWebPromisePtr<FResponseData<FDTSessionUpdate>> PostSessionResetWith(
        const FDTHttpContext& Context, const FString& SessionReg, const FDateTime& Date, const EDTPropertyQuery& Query, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("sessionReg"), SanitizeArg(LexToString(SessionReg)));
        Args.Add(TEXT("date"), SanitizeArg(LexToString(Date)));
        Args.Add(TEXT("query"), SanitizeArg(LexToString(Query)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FDTSessionUpdate>(
            Context, "POST", FString::Format(TEXT("/session/reset/{sessionReg}/from/{date}/{query}"), Args), RequestBody);
    }

    /** This endpoint lets non-registered players observe a session if enabled on this server and on entities that allow it.
It is restricted to only one entity and we always assume a full refresh to allow for caching. */
    static TWebPromisePtr<FResponseData<FDTSessionUpdate>> GetSessionSpectateWith(
        const FDTHttpContext& Context, const FString& EntityRef, const FDateTime& Date, const EDTPropertyQuery& Query, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("entityRef"), SanitizeArg(LexToString(EntityRef)));
        Args.Add(TEXT("date"), SanitizeArg(LexToString(Date)));
        Args.Add(TEXT("query"), SanitizeArg(LexToString(Query)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FDTSessionUpdate>(
            Context, "GET", FString::Format(TEXT("/session/spectate/{entityRef}/from/{date}/{query}"), Args), RequestBody);
    }

    /** Stop a session. This should be called once player disconnects. */
    static TWebPromisePtr<FResponseData<FEmptyResult>> DeleteSessionStopWith(
        const FDTHttpContext& Context, const FString& SessionReg, 
        const FEmptyBody& RequestBody)
    {
        TMap<FString, FStringFormatArg> Args;
        Args.Add(TEXT("sessionReg"), SanitizeArg(LexToString(SessionReg)));

        return UDTHttpRequester::SendHttpRequest<FEmptyBody, FEmptyResult>(
            Context, "DELETE", FString::Format(TEXT("/session/stop/{sessionReg}"), Args), RequestBody);
    }
};