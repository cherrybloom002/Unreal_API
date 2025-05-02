// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetActionUtility.h"
#include "Materials/Material.h"
#include "Materials/MaterialInstanceConstant.h"
#include "QuickAssetAction.generated.h"
/**
 * 
 */
UCLASS()
class QUICKACTIONS_API UQuickAssetAction : public UAssetActionUtility
{
	GENERATED_BODY()

public:
	UFUNCTION(CallInEditor)//function will be callable in editor
	void TestAction();

	UFUNCTION(CallInEditor)
	void Duplicate(int32 numCopies);

	UFUNCTION(CallInEditor)
	void FixPrefix();

	UFUNCTION(CallInEditor)
	void BatchRename(const FString& prefix = TEXT(""),
					 const FString& suffix = TEXT(""), 
					 const FString& prefixSeparator = TEXT(""), 
					 const FString& sufficSeparator = TEXT(""));

private:
	TMap<UClass*, FString> prefixMap = {
		{UBlueprint::StaticClass(), TEXT("BP_")},
		{UMaterial::StaticClass(), TEXT("M_")},
		{UMaterialInstanceConstant::StaticClass(), TEXT("MI_")}
	};
};
