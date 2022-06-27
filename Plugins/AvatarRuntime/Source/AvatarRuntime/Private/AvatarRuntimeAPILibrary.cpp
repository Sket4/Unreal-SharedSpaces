// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarRuntimeAPILibrary.h"
#include "HttpModule.h"
#include "IImageWrapper.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "Dom/JsonObject.h"
#include "Serialization/JsonReader.h"
#include "Serialization/JsonSerializer.h"
#include "IImageWrapperModule.h"
#include "Engine/Texture2D.h"

void UAvatarRuntimeAPILibrary::LoadAvatarIDArray(const FString& AuthToken, const FAvatarIDHttpResponse& Completed)
{
	if (AuthToken.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Auth token is empty!"));
		return;
	}

#if ENGINE_MINOR_VERSION > 25
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
#endif

	HttpRequest->SetURL(TEXT("https://app.gsize.io/v2/user_avatars/list_with_token"));
	HttpRequest->SetHeader(TEXT("accept"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AuthToken);
	HttpRequest->SetVerb(TEXT("POST"));

	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, const FAvatarIDHttpResponse& Completed)
		{
			if (!bConnectedSuccessfully || !Response.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Response failed"));
				return;
			}
		
			TArray<FString> IDArray;
			FString JsonString = Response->GetContentAsString();
			const int32 ResponseCode = Response->GetResponseCode();

			if (ResponseCode == 200)
			{
				JsonString = TEXT("{\"IDArray\":") + JsonString;
				JsonString = JsonString + TEXT("}");
				
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
				TSharedPtr<FJsonObject> JsonStructure = MakeShareable(new FJsonObject());
    
				if (FJsonSerializer::Deserialize(JsonReader, JsonStructure) && JsonStructure.IsValid())
				{
					const TArray<TSharedPtr<FJsonValue>>& JsonIDArray = JsonStructure->GetArrayField(TEXT("IDArray"));
					for (const TSharedPtr<FJsonValue> Value : JsonIDArray)
					{
						IDArray.Add(Value->AsString());
					}
				}
			}
		
			Completed.ExecuteIfBound(IDArray, JsonString, ResponseCode);
		}, Completed);

	HttpRequest->ProcessRequest();
}

void UAvatarRuntimeAPILibrary::GetAvatarGlbUrlByID(const FString& AuthToken, const FString& ID, const FAvatarGLBURLHttpResponse& Completed)
{
	if (ID.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ID is empty!"));
		return;
	}

	if (AuthToken.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Auth token is empty!"));
		return;
	}

#if ENGINE_MINOR_VERSION > 25
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
#endif

	HttpRequest->SetURL(TEXT("https://app.gsize.io/v2/user_avatars/model_with_token/") + ID + TEXT("?format=glb"));
	HttpRequest->SetHeader(TEXT("accept"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AuthToken);
	HttpRequest->SetVerb(TEXT("POST"));

	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, const FAvatarGLBURLHttpResponse& Completed, const FString& ID)
		{
			if (!bConnectedSuccessfully || !Response.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Response failed"));
				return;
			}
		
			FString URL;
			FString JsonString = Response->GetContentAsString();
			const int32 ResponseCode = Response->GetResponseCode();

			if (ResponseCode == 200)
			{
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
				TSharedPtr<FJsonObject> JsonStructure = MakeShareable(new FJsonObject());
    
				if (FJsonSerializer::Deserialize(JsonReader, JsonStructure) && JsonStructure.IsValid())
				{
					JsonStructure->TryGetStringField(TEXT("url"), URL);
				}
			}
		
			Completed.ExecuteIfBound(URL, JsonString, ResponseCode, ID);
		}, Completed, ID);

	HttpRequest->ProcessRequest();
}

void UAvatarRuntimeAPILibrary::GetAvatarPreviewUrlByID(const FString& AuthToken, const FString& ID,
	const FAvatarGLBURLHttpResponse& Completed)
{
	if (ID.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("ID is empty!"));
		return;
	}

	if (AuthToken.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Auth token is empty!"));
		return;
	}

#if ENGINE_MINOR_VERSION > 25
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
#endif

	HttpRequest->SetURL(TEXT("https://app.gsize.io/v2/user_avatars/preview_with_token/") + ID);
	HttpRequest->SetHeader(TEXT("accept"), TEXT("application/json"));
	HttpRequest->SetHeader(TEXT("Authorization"), TEXT("Bearer ") + AuthToken);
	HttpRequest->SetVerb(TEXT("POST"));

	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, const FAvatarGLBURLHttpResponse& Completed, const FString& ID)
		{
			if (!bConnectedSuccessfully || !Response.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Response failed"));
				return;
			}
		
			FString URL;
			FString JsonString = Response->GetContentAsString();
			const int32 ResponseCode = Response->GetResponseCode();

			if (ResponseCode == 200)
			{
				TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);
				TSharedPtr<FJsonObject> JsonStructure = MakeShareable(new FJsonObject());
    
				if (FJsonSerializer::Deserialize(JsonReader, JsonStructure) && JsonStructure.IsValid())
				{
					JsonStructure->TryGetStringField(TEXT("url"), URL);
				}
			}
		
			Completed.ExecuteIfBound(URL, JsonString, ResponseCode, ID);
		}, Completed, ID);

	HttpRequest->ProcessRequest();
}

void UAvatarRuntimeAPILibrary::LoadPreviewFromUrl(const FString& Url, const FString& ID, const FAvatarPreviewLoad& Completed)
{
	if (Url.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Url is empty!"));
		return;
	}

#if ENGINE_MINOR_VERSION > 25
	const TSharedRef<IHttpRequest, ESPMode::ThreadSafe> HttpRequest = FHttpModule::Get().CreateRequest();
#else
	TSharedRef<IHttpRequest> httpRequest = FHttpModule::Get().CreateRequest();
#endif

	HttpRequest->SetURL(Url);
	HttpRequest->SetHeader(TEXT("accept"), TEXT("application/json"));
	HttpRequest->SetVerb(TEXT("GET"));

	HttpRequest->OnProcessRequestComplete().BindLambda([](FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully, const FAvatarPreviewLoad& Completed, const FString& ID)
		{
			if (!bConnectedSuccessfully || !Response.IsValid())
			{
				UE_LOG(LogTemp, Warning, TEXT("Response failed"));
				return;
			}
		
			TArray<uint8> BytesArray = Response->GetContent();
			const int32 ResponseCode = Response->GetResponseCode();

			UTexture2D* Texture = nullptr;

			if (ResponseCode == 200)
			{
				IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
				auto ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

				if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(BytesArray.GetData(), BytesArray.Num()))
				{
					TArray<uint8> UncompressedBGRA;
					if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
					{
						const int32 Height = ImageWrapper->GetHeight();
						const int32 Width = ImageWrapper->GetWidth();
						
						Texture = UTexture2D::CreateTransient(Width, Height, PF_B8G8R8A8);

#if WITH_EDITORONLY_DATA
						Texture->MipGenSettings = TMGS_NoMipmaps;
#endif
						
						void* TextureData = Texture->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
						FMemory::Memcpy(TextureData, UncompressedBGRA.GetData(), Texture->PlatformData->Mips[0].BulkData.GetBulkDataSize());
						Texture->PlatformData->Mips[0].BulkData.Unlock();
 					
						Texture->UpdateResource();
					}
				}
			}
		
			Completed.ExecuteIfBound(ID, Texture);
		}, Completed, ID);

	HttpRequest->ProcessRequest();
}

void UAvatarRuntimeAPILibrary::DestroyTextures(TArray<UTexture2D*>& Textures, UObject* WorldContext)
{
	if (!WorldContext)
		return;
	
	for (auto Texture : Textures)
	{
		Texture->ConditionalBeginDestroy();
		Texture = nullptr;
	}

	Textures.Empty();
	GEngine->ForceGarbageCollection(true);
}
