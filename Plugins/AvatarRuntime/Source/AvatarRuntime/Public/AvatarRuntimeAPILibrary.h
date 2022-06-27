// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AvatarRuntimeAPILibrary.generated.h"

DECLARE_DYNAMIC_DELEGATE_ThreeParams(FAvatarIDHttpResponse,
	const TArray<FString>&, IDArray,
	const FString&, RawResponseContent,
	const int32&, ResponseCode);

DECLARE_DYNAMIC_DELEGATE_FourParams(FAvatarGLBURLHttpResponse,
	const FString&, URL,
	const FString&, RawResponseContent,
	const int32&, ResponseCode,
	const FString&, ID);

DECLARE_DYNAMIC_DELEGATE_TwoParams(FAvatarPreviewLoad,
	const FString&, ID,
 	UTexture2D*, Texture);

UCLASS()
class AVATARRUNTIME_API UAvatarRuntimeAPILibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void LoadAvatarIDArray(const FString& AuthToken, const FAvatarIDHttpResponse& Completed);

	UFUNCTION(BlueprintCallable)
	static void GetAvatarGlbUrlByID(const FString& AuthToken, const FString& ID, const FAvatarGLBURLHttpResponse& Completed);

	UFUNCTION(BlueprintCallable)
	static void GetAvatarPreviewUrlByID(const FString& AuthToken, const FString& ID, const FAvatarGLBURLHttpResponse& Completed);

	UFUNCTION(BlueprintCallable)
	static void LoadPreviewFromUrl(const FString& Url, const FString& ID, const FAvatarPreviewLoad& Completed);

	UFUNCTION(BlueprintCallable)
	static void DestroyTextures(UPARAM(ref)TArray<UTexture2D*>& Textures, UObject* WorldContext);
};
