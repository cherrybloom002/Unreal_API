// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogQuickActions, Log, All);

class FQuickActionsModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
#pragma region ContentBrowserExtension
	void InitCBExtension();
	TSharedRef<FExtender> CustomCBExtender(const TArray<FString>& SelectedPaths);

	TArray<FString> SelectedFolderPaths;

	void AddCBMenuEntry(FMenuBuilder& MenuBuilder);
	void OnDeleteUnusedAssetsClicked();
#pragma endregion
};
