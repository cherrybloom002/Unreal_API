// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

struct ListAsset
{
	FString Type;
	FString Path;
	ListAsset(FString type, FString path): Type(type), Path(path) {}
};

class SSceneAssetViewerWidget : public SCompoundWidget
{

	SLATE_BEGIN_ARGS(SSceneAssetViewerWidget) {}
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& inArgs);

private:
	void PopulateAssetSet();

	TSet<FString> BlueprintSet;
	TSet<FString> MeshesSet;
	TSet<FString> MatsSet;

	TArray<TSharedPtr<ListAsset>> AssetList;

	TSharedPtr<SListView<TSharedPtr<ListAsset>>> ListViewWidget;

	TSharedRef<ITableRow> OnGeneratedRow(TSharedPtr<ListAsset> item, const TSharedRef<STableViewBase>& OwnerTable);
};
