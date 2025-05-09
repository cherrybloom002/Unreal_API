// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickActions.h"
#include "ContentBrowserModule.h"
#include "../DebugHeader.h"
#define LOCTEXT_NAMESPACE "FQuickActionsModule"

DEFINE_LOG_CATEGORY(LogQuickActions)

void FQuickActionsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	InitCBExtension();
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
		//3- Effettiva Funzione

		/*2 Examples of Menu extensions:
			The first one will be positioned as the first in the "Bulk Operations" cathegory
			while the second one will be after "Delete"*/
		MenuExtender->AddMenuExtension(FName("PathContextBulkOperations"), //1- Hook Menu
			EExtensionHook::First,
			TSharedPtr<FUICommandList>(), //quick commands
			FMenuExtensionDelegate::CreateRaw(this, &FQuickActionsModule::AddCBMenuEntry) //2- MenuBuild
		);
		MenuExtender->AddMenuExtension(FName("Delete"), //1- Hook Menu
			EExtensionHook::After, 
			TSharedPtr<FUICommandList>(),
			FMenuExtensionDelegate::CreateRaw(this, &FQuickActionsModule::AddCBMenuEntry) //2- MenuBuild
			);
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
}
void FQuickActionsModule::OnDeleteUnusedAssetsClicked()
{
	DebugHeader::ScreenPrint(TEXT("Clicked"), FColor::Purple);
}
#pragma endregion

void FQuickActionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuickActionsModule, QuickActions)