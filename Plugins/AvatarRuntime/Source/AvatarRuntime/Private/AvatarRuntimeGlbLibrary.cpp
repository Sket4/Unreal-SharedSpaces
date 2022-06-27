// Fill out your copyright notice in the Description page of Project Settings.


#include "AvatarRuntimeGlbLibrary.h"

FglTFRuntimeConfig UAvatarRuntimeGlbLibrary::GetGLTFRuntimeConfig()
{
	FglTFRuntimeConfig runtimeConfig;
	runtimeConfig.TransformBaseType = EglTFRuntimeTransformBaseType::YForward;
	return runtimeConfig;
}

FglTFRuntimeSkeletalMeshConfig UAvatarRuntimeGlbLibrary::GetGLTFRuntimeSkeletalMeshConfig(FString& rootBoneName,
	USkeleton* skeleton)
{
	FglTFRuntimeSkeletalMeshConfig skeletonMeshConfig;
	skeletonMeshConfig.SkeletonConfig = GetGLTFRuntimeSkeletonConfig(TEXT("Armature"), skeleton);

	rootBoneName = TEXT("Armature");
	skeletonMeshConfig.Skeleton = skeleton;
	skeletonMeshConfig.MaterialsConfig = GetGLTFRuntimeMaterialsConfig();
	return skeletonMeshConfig;
}

FglTFRuntimeSkeletonConfig UAvatarRuntimeGlbLibrary::GetGLTFRuntimeSkeletonConfig(const FString& rootBoneName,
	USkeleton* skeleton)
{
	FglTFRuntimeSkeletonConfig skeletonConfig;
	skeletonConfig.CopyRotationsFrom = skeleton;
	skeletonConfig.bAddRootBone = true;
	skeletonConfig.RootBoneName = rootBoneName;
	return skeletonConfig;
}

FglTFRuntimeMaterialsConfig UAvatarRuntimeGlbLibrary::GetGLTFRuntimeMaterialsConfig()
{
	FglTFRuntimeMaterialsConfig materialsConfig;
	materialsConfig.bMergeSectionsByMaterial = true;
	return materialsConfig;
}

USkeletalMeshComponent* UAvatarRuntimeGlbLibrary::SetupSDKSkeleton(AActor* target, USkeletalMesh* skeletalMesh,
	UClass* animBlueprintInstance)
{
	USkeletalMeshComponent* skeletalMeshComponent = AddSkeletalMeshComponent(target, skeletalMesh);
	if (animBlueprintInstance)
	{
		SetAnimationBlueprint(skeletalMeshComponent, animBlueprintInstance);
	}

	return skeletalMeshComponent;
}

USkeletalMeshComponent* UAvatarRuntimeGlbLibrary::AddSkeletalMeshComponent(AActor* target, USkeletalMesh* skeletalMesh)
{
	USkeletalMeshComponent* skeletalMeshComponent = NewObject<USkeletalMeshComponent>(target, TEXT("Skeletal Mesh"));
	skeletalMeshComponent->SetupAttachment(target->GetRootComponent());
	skeletalMeshComponent->RegisterComponent();
	skeletalMeshComponent->SetSkeletalMesh(skeletalMesh);

	return skeletalMeshComponent;
}

void UAvatarRuntimeGlbLibrary::SetAnimationBlueprint(USkeletalMeshComponent* skeletalMeshComponent,
	UClass* animInstance)
{
	skeletalMeshComponent->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	skeletalMeshComponent->SetAnimInstanceClass(animInstance);
}
