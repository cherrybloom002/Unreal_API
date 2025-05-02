// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetActions/QuickAssetAction.h"
#include "../DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"
#include "AssetToolsModule.h"
#include "AssetViewUtils.h"
#include "AssetRegistry/AssetRegistryModule.h"

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

void UQuickAssetAction::BatchRename(const FString& prefix, const FString& suffix, const FString& prefixSeparator, const FString& sufficSeparator)
{
	if (prefix.IsEmpty() && suffix.IsEmpty())
	{
		FixPrefix();
		return;
	}

	TArray<UObject*> selectedAssets = UEditorUtilityLibrary::GetSelectedAssets();
	uint32 counter = 0;

	for (UObject* obj : selectedAssets)
	{
		if (!obj)
		{
			continue;
		}

		FString* epicGamesPrefix = prefixMap.Find(obj->GetClass());
		if (!epicGamesPrefix || epicGamesPrefix->IsEmpty())
		{
			LogPrint(TEXT("Failed to find Epic Games prefix for class " + obj->GetClass()->GetName()));
			continue;
		}

		const FString oldName = obj->GetName();
		FString newName = oldName;

		if (!prefix.IsEmpty())
		{
			if (oldName.StartsWith(*epicGamesPrefix))
			{
				LogPrint(TEXT("Already has a valid Epic Games prefix: " + oldName));
				newName.InsertAt(epicGamesPrefix->Len(), (prefix + prefixSeparator));
			}
			else
			{
				newName.InsertAt(0, *epicGamesPrefix);
				newName.InsertAt(epicGamesPrefix->Len(), (prefix + prefixSeparator));
			}
		}
		else
		{
			if (oldName.StartsWith(*epicGamesPrefix))
			{
				LogPrint(TEXT("Already has a valid Epic Games prefix: " + oldName));
			}
			else
			{
				newName.InsertAt(0, *epicGamesPrefix);
			}
		}

		if (!suffix.IsEmpty())
		{
			newName.Append(sufficSeparator + suffix);
		}

		UEditorUtilityLibrary::RenameAsset(obj, newName);
		++counter;
	}

	if (counter > 0)
	{
		ShowNotifyInfo(TEXT("Successfully renamed " + FString::FromInt(counter) + " assets!"));
		LogPrint(TEXT("Successfully renamed " + FString::FromInt(counter) + " assets!"));
	}
}

void UQuickAssetAction::RemoveUnused()
{
	FixRedirectors();
	TArray<FAssetData> selectedAssets = UEditorUtilityLibrary::GetSelectedAssetData();
	TArray<FAssetData> unusedAssets;

	for (const FAssetData& data : selectedAssets)
	{
		TArray<FString> references = UEditorAssetLibrary::FindPackageReferencersForAsset(data.ObjectPath.ToString());
		if (references.Num() == 0) 
		{
			unusedAssets.Add(data);
		}
	}
	if (unusedAssets.Num() == 0) {
		ShowDialog(EAppMsgType::Ok, TEXT("No unused assets selected"));
		return;
	}
	const int32 deletedObjs = ObjectTools::DeleteAssets(unusedAssets);
	if (deletedObjs == 0) return;
	ShowNotifyInfo(TEXT("Successfully deleted " + FString::FromInt(deletedObjs) + " unused assets!"));
}

void UQuickAssetAction::FixRedirectors()
{
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();

	FARFilter Filter;
	Filter.bRecursivePaths = true;
	Filter.PackagePaths.Emplace("/Game");
	Filter.ClassPaths.Add(UObjectRedirector::StaticClass()->GetClassPathName());

	TArray<FAssetData> AssetList;
	AssetRegistry.GetAssets(Filter, AssetList);

	if (AssetList.Num() == 0) return;

	TArray<FString> ObjectPaths;

	for (const FAssetData& data : AssetList)
	{
		ObjectPaths.Add(data.GetObjectPathString());
	}

	TArray<UObject*> objs;
	bool Result = AssetViewUtils::LoadAssetsIfNeeded(ObjectPaths, objs, true, true);

	if (Result)
	{
		TArray<UObjectRedirector*> Redirectors;
		for (UObject* object : objs)
		{
			Redirectors.Add(CastChecked<UObjectRedirector>(object));
		}
		 
		IAssetTools& AssetToolModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>(TEXT("AssetsTools")).Get();

		AssetToolModule.FixupReferencers(Redirectors);
	}

}

