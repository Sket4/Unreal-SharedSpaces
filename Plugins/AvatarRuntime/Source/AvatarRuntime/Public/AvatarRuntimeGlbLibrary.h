// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "glTFRuntimeParser.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AvatarRuntimeGlbLibrary.generated.h"


UCLASS()
class AVATARRUNTIME_API UAvatarRuntimeGlbLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static FglTFRuntimeConfig GetGLTFRuntimeConfig();

	UFUNCTION(BlueprintCallable)
	static FglTFRuntimeSkeletalMeshConfig GetGLTFRuntimeSkeletalMeshConfig(FString& rootBoneName,
		USkeleton* skeleton = nullptr);

	UFUNCTION(BlueprintCallable)
	static FglTFRuntimeSkeletonConfig GetGLTFRuntimeSkeletonConfig(const FString& rootBoneName, USkeleton* skeleton);

	UFUNCTION(BlueprintCallable)
	static FglTFRuntimeMaterialsConfig GetGLTFRuntimeMaterialsConfig();

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Target"))
	static USkeletalMeshComponent* SetupSDKSkeleton(AActor* target, USkeletalMesh* skeletalMesh, UClass* animBlueprintInstance);

	UFUNCTION(BlueprintCallable, meta = (DefaultToSelf = "Target"))
	static USkeletalMeshComponent* AddSkeletalMeshComponent(AActor* target, USkeletalMesh* skeletalMesh);

	UFUNCTION(BlueprintCallable)
	static void SetAnimationBlueprint(USkeletalMeshComponent* skeletalMeshComponent, UClass* animInstance);
};
