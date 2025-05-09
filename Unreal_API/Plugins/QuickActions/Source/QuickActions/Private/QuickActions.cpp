// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickActions.h"
#include "ContentBrowserModule.h"
#include "../DebugHeader.h"
#include "EditorUtilityLibrary.h"
#include "EditorAssetLibrary.h"
#include "ObjectTools.h"

#define LOCTEXT_NAMESPACE "FQuickActionsModule"

DEFINE_LOG_CATEGORY(LogQuickActions)

void FQuickActionsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBExtension();

	RegisterSlateTab();
}

#pragma region ContentBrowserExtension
void FQuickActionsModule::InitCBExtension()
{
	FContentBrowserModule& CBModule = FModuleManager::LoadModuleChecked<FContentBrowserModule>(TEXT("ContentBrowser"));

	TArray<FContentBrowserMenuExtender_SelectedPaths>& CBExtender = CBModule.GetAllPathViewContextMenuExtenders();

	FContentBrowserMenuExtender_SelectedPaths CustomCBMenuDelegate;

	CustomCBMenuDelegate.BindRaw(this, &FQuickActionsModule::CustomCBExtender);

	CBExtender.Add(CustomCBMenuDelegate);

}
TSharedRef<FExtender> FQuickActionsModule::CustomCBExtender(const TArray<FString>& SelectedPaths)
{
	TSharedRef<FExtender> MenuExtender(new FExtender());

	if (SelectedPaths.Num() > 0)
	{
		//1- Hook Menu
		//2- MenuBuild
		//3- Function

		/*2 Examples of Menu extensions:
			The first one will be positioned as the first in the "Bulk Operations" cathegory
			while the second one will be after "Delete"*/
		MenuExtender->AddMenuExtension(FName("PathContextBulkOperations"), //1- Hook Menu
			EExtensionHook::First,
			TSharedPtr<FUICommandList>(), //quick commands
			FMenuExtensionDelegate::CreateRaw(this, &FQuickActionsModule::AddCBMenuEntry) //2- MenuBuild
		);
		/*MenuExtender->AddMenuExtension(FName("Delete"), //1- Hook Menu
			EExtensionHook::After, 
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FQuickActionsModule::AddCBMenuEntry) //2- MenuBuild
			);*/
		SelectedFolderPaths = SelectedPaths;
	}
	return MenuExtender;
}
void FQuickActionsModule::AddCBMenuEntry(FMenuBuilder& MenuBuilder)
{
	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Delete Unused Assets")),
		FText::FromString(TEXT("Delete unused asset in folder")),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"),
		FExecuteAction::CreateRaw(this, &FQuickActionsModule::OnDeleteUnusedAssetsClicked)
	);

	MenuBuilder.AddMenuEntry(
		FText::FromString(TEXT("Spawn test nomad")),
		FText::FromString(TEXT("Slate test nomad spawn")),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "LevelEditor.Tabs.Details"),
		FExecuteAction::CreateRaw(this, &FQuickActionsModule::OnSpawnTabClicked)
	);

	
}
void FQuickActionsModule::OnDeleteUnusedAssetsClicked()// 3- Function
{
	DebugHeader::ScreenPrint(TEXT("Clicked"), FColor::Purple);
	if (SelectedFolderPaths.Num() > 1)
	{
		DebugHeader::ShowDialog(EAppMsgType::Ok, TEXT("Only one folder")); 
		return;
	}

	TArray<FString> AssetsPath = UEditorAssetLibrary::ListAssets(SelectedFolderPaths[0]);
	if (AssetsPath.Num() == 0)
	{
		DebugHeader::ShowDialog(EAppMsgType::Ok, TEXT("Empty Folder"));
		return;
	}
	EAppReturnType::Type Confirmation = DebugHeader::ShowDialog(EAppMsgType::YesNo, TEXT("Found ") + FString::FromInt(AssetsPath.Num()) + TEXT("\nProceed?"));
	if (Confirmation == EAppReturnType::No)
	{
		return;
	}

	TArray<FAssetData>UnusedAssets;
	for (const FString& path : AssetsPath)
	{
		if (path.Contains(TEXT("Developers")) || path.Contains(TEXT("Collections")))
		{
			continue;
		}
		if (!UEditorAssetLibrary::DoesAssetExist(path))continue;

		TArray<FString> References = UEditorAssetLibrary::FindPackageReferencersForAsset(path);

		if (References.Num() ==0)
		{
			const FAssetData unusedData = UEditorAssetLibrary::FindAssetData(path);
			UnusedAssets.Add(unusedData);
		}
	}
	if (UnusedAssets.Num() > 0)
	{
		ObjectTools::DeleteAssets(UnusedAssets);
	}
	else
	{
		DebugHeader::ShowDialog(EAppMsgType::Ok, TEXT("No unused assets"));
		return;
	}

}
void FQuickActionsModule::RegisterSlateTab()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(
		FName("TestTab"),
		FOnSpawnTab::CreateRaw(this, &FQuickActionsModule::OnSpawnTestTab))
		.SetDisplayName(FText::FromString(TEXT("Nomad tab test")));
}
void FQuickActionsModule::OnSpawnTabClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(FName("TestTab"));
}
TSharedRef<SDockTab> FQuickActionsModule::OnSpawnTestTab(const FSpawnTabArgs& args)
{
	return SNew(SDockTab).TabRole(ETabRole::NomadTab);
}
#pragma endregion
#pragma region SlateTest
void FQuickActionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}
#pragma endregion
#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuickActionsModule, QuickActions)