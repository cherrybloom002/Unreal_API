// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Widgets/SCompoundWidget.h"

struct ListViewData
{
	ListViewData(FString a, FString b): foo(a), bar(b){}
	FString foo;
	FString bar;
};
class SMyCustomWidget : public SCompoundWidget
{
	SLATE_BEGIN_ARGS(SMyCustomWidget) {}
	SLATE_ARGUMENT(FString, TestString)
	SLATE_END_ARGS()

public:
	void Construct(const FArguments& inArgs);
private:
	float TestValue;
	float OnGetTestValue()const { return TestValue; }
	FReply HandleButtonClick();
	bool bIsChecked;

	FString CurrentText;
	void HandleTextChanged(const FText& newText);
	void HandleTextCommitted(const FText& newText, ETextCommit::Type CommitType);

	TArray<TSharedPtr<FString>> Options_CB;
	TSharedPtr<FString> Selected_CB;
	TSharedRef<SWidget> GenerateComboBoxItem(TSharedPtr<FString> item);
	void HandleSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo);
	FText GetCurrentSelectionCB() const;

	float value;
	void HandleSliderValueChange(float newValue);

	TSharedRef<ITableRow> GeneratedListRow(TSharedPtr<FString> item,const TSharedRef<STableViewBase> & OwnerTable);
	void HandleListSelectionChanged(TSharedPtr<FString> selectedItem, ESelectInfo::Type SelectInfo);

	TArray<TSharedPtr<ListViewData>> Options_LV;
	TSharedRef<ITableRow> GeneratedListRowLVD(TSharedPtr<ListViewData> item, const TSharedRef<STableViewBase>& OwnerTable);
	void HandleListSelectionChangedLVD(TSharedPtr<ListViewData> selectedItem, ESelectInfo::Type SelectInfo);
};
