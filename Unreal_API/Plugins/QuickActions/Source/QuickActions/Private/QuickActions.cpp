// Copyright Epic Games, Inc. All Rights Reserved.

#include "QuickActions.h"
#include "ContentBrowserModule.h"
#define LOCTEXT_NAMESPACE "FQuickActionsModule"

DEFINE_LOG_CATEGORY(LogQuickActions)

void FQuickActionsModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
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
		//2- MenuBuilding
		//3- Effettiva Funzione
		//MenuExtender->AddMenuExtension();
	}
	return MenuExtender;
}
#pragma endregion

void FQuickActionsModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FQuickActionsModule, QuickActions)