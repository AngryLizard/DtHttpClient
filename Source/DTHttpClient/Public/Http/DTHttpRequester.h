
#pragma once

#include "DTHttpContext.h"
#include "Generated/DTModels.h"
#include "Structures/CallPromise.h"
#include "Structures/CallPromiseQueue.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "DTHttpClient.h"

#include "DTHttpWebUtils.h"
#include "DTHttpWebPromise.h"

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DTHttpRequester.generated.h"

template<typename ResponseType>
struct FResponseData
{
	ResponseType Body;
	EHttpResponseCodes::Type Code;
	FDTResponseMessage Message;

	bool IsOk() const { return EHttpResponseCodes::IsOk(Code); }
	void Report(const FString& Header) const
	{
		if (IsOk())
		{
			const FString BodyString = LexToString(Body);
			UE_LOG(DTHttpClient, Error, TEXT("%s\nSuccess (200)\n%s"), *Header, *BodyString);
			return;
		}
		UE_LOG(DTHttpClient, Error, TEXT("%s\n%s (%d)\n%s"), *Header, *Message.Name, Code, *Message.Message);
	}
};

/**
 *
 */
UCLASS()
class DTHTTPCLIENT_API UDTHttpRequester : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	using FRequestPtr = TSharedPtr<class IHttpRequest, ESPMode::ThreadSafe>;

	static TWebPromisePtr<FResponseString> SendHttpRequest(
		const FDTHttpContext& Context,
		const FString& Verb, 
		const FString& Path, 
		const FString& Body);

	template<typename RequestBodyType, typename ResultType>
	static TWebPromisePtr<FResponseData<ResultType>> SendHttpRequest(
		const FDTHttpContext& Context,
		const FString& Verb, 
		const FString& Path, 
		const RequestBodyType& Body)
	{
		const FString BodyString = LexToString(Body);
		UE_LOG(DTHttpClient, Log, TEXT("Calling %s:%s with Body %s"), *Verb, *Path, (BodyString.IsEmpty() ? TEXT("empty") : *BodyString));
		return SendHttpRequest(Context, Verb, Path, BodyString)
			->Then([](const FResponseString& Result)
			{
				FResponseData<ResultType> ResponseData;
				ResponseData.Code = static_cast<EHttpResponseCodes::Type>(Result.Code);

				if (ResponseData.IsOk())
				{
					LexFromString(ResponseData.Body, *Result.Body);
				}
				else
				{
					LexFromString(ResponseData.Message, *Result.Body);
				}

				return ResponseData;
			});
	}

	template<typename ResultType>
	static TWebPromisePtr<FResponseData<ResultType>> SendHttpRequest(
		const FDTHttpContext& Context,
		const FString& Verb,
		const FString& Path,
		const TSharedRef<FJsonObject>& Body)
	{
		FString BodyString;
		if (!FJsonSerializer::Serialize(Body, TJsonWriterFactory<>::Create(&BodyString, 0)))
		{
			UE_LOG(DTHttpClient, Error, TEXT("Json serialization failed!"));
			return TWebPromise<FResponseData<ResultType>>::Rejected(FWebError());
		}

		UE_LOG(DTHttpClient, Log, TEXT("Calling %s:%s with Body %s"), *Verb, *Path, (BodyString.IsEmpty() ? TEXT("empty") : *BodyString));
		return SendHttpRequest(Context, Verb, Path, BodyString)
			->Then([](const FResponseString& Result)
			{
				FResponseData<ResultType> ResponseData;
				ResponseData.Code = static_cast<EHttpResponseCodes::Type>(Result.Code);

				if (ResponseData.IsOk())
				{
					LexFromString(ResponseData.Body, *Result.Body);
				}
				else
				{
					LexFromString(ResponseData.Message, *Result.Body);
				}

				return ResponseData;
			});
	}

	/////////////////////////////////////////////////////////////////////////////

	using FSSEStopEvent = TMulticastDelegate<void()>;

	struct FSSEContext {

		FSSEContext(FRequestPtr InRequest);
		FString Consume();
		void Stop();

		FSSEStopEvent OnStop;

	private:
		FRequestPtr Request;
		int64 DataRead;
	};

	using FSSEContextPtr = TSharedPtr<FSSEContext>;
	using FSSEResponseCallback = TDelegate<void(const FString&)>;

	static FSSEContextPtr StartSSESession(
		const FDTHttpContext& Context,
		const FString& Path,
		int32 MaxBytes,
		float Timeout,
		FSSEResponseCallback Callback);

	template<typename ResultType>
	using FSSEResultCallback = TDelegate<void(const ResultType&)>;

	template<typename ResultType>
	static FSSEContextPtr StartSSESession(
		const FDTHttpContext& Context, 
		const FString& Path,
		int32 MaxBytes,
		float Timeout,
		FSSEResultCallback<ResultType> Callback)
	{
		return StartSSESession(Context, Path, MaxBytes, Timeout,
			FSSEResponseCallback::CreateLambda([Callback](const FString& Result)
			{
				ResultType Response;
				LexFromString(Response, *Result);
				Callback.ExecuteIfBound(Response);
			}));
	}
};
