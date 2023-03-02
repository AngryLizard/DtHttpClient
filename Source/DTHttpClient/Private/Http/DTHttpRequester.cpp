
#include "Http/DTHttpRequester.h"
#include "HttpModule.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "DTHttpClient.h"

TWebPromisePtr<FResponseString> UDTHttpRequester::SendHttpRequest(
    const FDTHttpContext& Context, 
    const FString& Verb, 
    const FString& Path, 
    const FString& Body)
{
    TWebPromisePtr<FResponseString> Promise = TWebPromise<FResponseString>::Create();

    FHttpModule& HttpModule = FHttpModule::Get();
    FRequestPtr Request = HttpModule.CreateRequest();

    Request->SetVerb(Verb);
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("application/json"));
    Request->SetHeader(TEXT("jwt"), Context.Token);
    Request->SetContentAsString(Body);
    Request->SetURL(Context.BaseUrl + Path);
    Request->OnProcessRequestComplete().BindLambda(
        [Promise](FHttpRequestPtr Request, FHttpResponsePtr Response, bool Success)
        {
            // Make absolutely sure this is running on game thread so we don't get race conditions
            AsyncTask(ENamedThreads::GameThread, [Promise, Request, Response, Success]()
            {
                if (Request->GetStatus() == EHttpRequestStatus::Failed_ConnectionError)
                {
                    UE_LOG(DTHttpClient, Error, TEXT("Connection failed."));
                    Promise->Reject(FWebError());
                    return;
                }

                FResponseString Result;
                Result.Code = Response->GetResponseCode();
                Result.Body = Response->GetContentAsString();
                Promise->Accept(Result);
            });
        });

    Request->ProcessRequest();
    return Promise;
}

UDTHttpRequester::FSSEContext::FSSEContext(FRequestPtr InRequest)
    : Request(InRequest), DataRead(0)
{
}

FResponseString UDTHttpRequester::FSSEContext::Consume()
{
    if (!Request.IsValid())
    {
        return FResponseString();
    }

    FHttpResponsePtr Response = Request->GetResponse();
    if (!Response.IsValid())
    {
        return FResponseString();
    }

    FResponseString Result;
    Result.Code = Response->GetResponseCode();

    const TArray<uint8>& Content = Response->GetContent();
    const int32 TotalBytes = Content.Num();
    if (DataRead >= TotalBytes)
    {
        return FResponseString();
    }

    TArrayView<const uint8> View(Content.GetData() + DataRead, TotalBytes - DataRead);
    TArray<uint8> ZeroTerminatedPayload(View);
    ZeroTerminatedPayload.Add(0);
    Result.Body = UTF8_TO_TCHAR(ZeroTerminatedPayload.GetData());

    DataRead = TotalBytes;
    return Result;
}

void UDTHttpRequester::FSSEContext::Stop()
{
    Request->CancelRequest();
}

UDTHttpRequester::FSSEContextPtr UDTHttpRequester::StartSSESession(
    const FDTHttpContext& Context,
    const FString& Path,
    FSSEResponseCallback Callback)
{
    FHttpModule& HttpModule = FHttpModule::Get();

    FRequestPtr Request = HttpModule.CreateRequest();

    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetHeader(TEXT("Accept"), TEXT("text/event-stream"));
    Request->SetHeader(TEXT("jwt"), Context.Token);
    Request->SetURL(Context.BaseUrl + Path);

    FSSEContextPtr SSEContext = MakeShared<FSSEContext>(Request);

    //Request->OnRequestProgress();
    UWorld* World = GEngine->GetCurrentPlayWorld();
    if (ensure(World))
    {
        Request->OnRequestProgress().BindLambda(
            [Callback, SSEContext](FHttpRequestPtr HttpRequest, int32 BytesSent, int32 BytesReceived)
            {
                if (SSEContext.IsValid())
                {
                    FResponseString Result = SSEContext->Consume();
                    Callback.ExecuteIfBound(Result);
                }
            });
    }

    Request->ProcessRequest();
    return SSEContext;

    /*
#include <curl/curl.h>

size_t sse_callback(char* data, size_t size, size_t nmemb, void* userdata) {
    // Parse and handle the SSE data received from the server
    std::string sse_data(data, size * nmemb);
    std::cout << "Received SSE data: " << sse_data << std::endl;

    // Return the number of bytes processed
    return size * nmemb;
}

    CURL* curl = curl_easy_init();
    if (curl) {
        // Set the SSE endpoint URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://example.com/sse_endpoint");

        // Set the "Accept: text/event-stream" header
        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Accept: text/event-stream");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Set the callback function for receiving SSE data
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, sse_callback);

        // Enable automatic decompression of the response
        curl_easy_setopt(curl, CURLOPT_ACCEPT_ENCODING, "");

        // Perform the SSE request and wait for data
        CURLcode res;
        do {
            res = curl_easy_perform(curl);
        } while (res == CURLE_OK);

        // Clean up
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        
    }*/
}