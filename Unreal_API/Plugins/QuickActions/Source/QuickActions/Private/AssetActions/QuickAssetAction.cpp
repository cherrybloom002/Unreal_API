// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "../DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"

void UQuickAssetAction::TestAction()
{
	ScreenPrint(TEXT("Work as Intended 2.0"), FColor::Purple);
	LogPrint(TEXT("Work as intended but in log"));
}

void UQuickAssetAction::Duplicate(int32 numCopies)
{

	if (numCopies <= 0) 
	{
		LogPrint(TEXT("Please Enter a valid number (>0)"));
		ShowDialog(EAppMsgType::Ok, TEXT("Please Enter a valid number (>0)"));
		return;
	}

	TArray<FAssetData> selectedAssetsData = UEditorUtilityLibrary::GetSelectedAssetData();
	uint32 counter = 0;

	for (const FAssetData& data : selectedAssetsData)
	{
		for (int32 i=0; i< numCopies; i++)
		{
			const FString SourceAssetPath = data.ObjectPath.ToString();
			const FString NewAssetName = data.AssetName.ToString() + TEXT("_Mk") + FString::FromInt(i+1);

			const FString DestinationAssetPath = FPaths::Combine(data.PackagePath.ToString(), NewAssetName);

			if (UEditorAssetLibrary::DuplicateAsset(SourceAssetPath, DestinationAssetPath))
			{
				UEditorAssetLibrary::SaveAsset(DestinationAssetPath, false);
				++counter;
			}
		}
	}

	if (counter > 0) 
	{
		ShowNotifyInfo(TEXT("Successfully created " + FString::FromInt(counter) + " assets"));
		LogPrint(TEXT("Successfully created " + FString::FromInt(counter) + " assets"));
	}
}

void UQuickAssetAction::FixPrefix()
{
	TArray<UObject*> selectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 counter = 0;

	for (UObject* obj : selectedAssets)
	{
		if (!obj) continue;
		FString* prefix = prefixMap.Find(obj->GetClass());
		if (!prefix || prefix->IsEmpty()) 
		{
			LogPrint(TEXT("Failed to find prefix for class " + obj->GetClass()->GetName()));
			continue;
		}
		FString oldName = obj->GetName();
		if (oldName.StartsWith(*prefix))
		{
			LogPrint(oldName +  TEXT(" already has a valid prefix"));
			continue;
		}
		if (obj->IsA<UMaterialInstanceConstant>())
		{
			oldName.RemoveFromStart("M_");
			oldName.RemoveFromEnd("_Inst");
		}
		const FString newName = *prefix + oldName;
		UEditorUtilityLibrary::RenameAsset(obj, newName);
		++counter;
	}

	if (counter > 0)
	{
		ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(counter) + " assets"));
		LogPrint(TEXT("Successfully renamed " + FString::FromInt(counter) + " assets"));
	}
}
