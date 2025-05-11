// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/MyCustomWidget.h"
#include "../DebugHeader.h"
#include "Widgets/Input/SSlider.h"
void SMyCustomWidget::Construct(const FArguments& inArgs)
{
	bCanSupportFocus = true;

	inArgs._TestString;
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleFont.Size = 30;
	Options_CB.Add(MakeShared<FString>("foo"));
	Options_CB.Add(MakeShared<FString>("bar"));
	Options_CB.Add(MakeShared<FString>("fuzz"));

	Options_LV.Add(MakeShared<ListViewData>("Lorem", "ipsum"));
	Options_LV.Add(MakeShared<ListViewData>("Lorem", "ipsum"));
	Options_LV.Add(MakeShared<ListViewData>("Lorem", "ipsum"));

	ChildSlot
	[
		SNew(SVerticalBox)
		//first slot
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(STextBlock)
				.Text(FText::FromString(inArgs._TestString))
				.Font(TitleFont)
				.ColorAndOpacity(FColor::White)
				.Justification(ETextJustify::Center)
		]

		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SButton)
				.Text(FText::FromString("Click me"))
				.OnClicked(this, &SMyCustomWidget::HandleButtonClick)
				.ButtonStyle(&FAppStyle::Get().GetWidgetStyle<FButtonStyle>("Menu.Button"))
		]

		+SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+SHorizontalBox::Slot()
				[
					SNew(SCheckBox)
						.OnCheckStateChanged_Lambda([this](ECheckBoxState newState)
						{
							bIsChecked = (newState == ECheckBoxState::Checked);
						})
				]
				+SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString("Test CheckBox"))
				]	
		]
		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString("Test Editable TextBox"))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SEditableTextBox)
						.HintText(FText::FromString("Test"))
						.OnTextChanged(this, &SMyCustomWidget::HandleTextChanged)
						.OnTextCommitted(this, &SMyCustomWidget::HandleTextCommitted)
				]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString("Test ComboBox"))
				]
				+ SHorizontalBox::Slot()
				[
					//TSharedRef<SComboBox<TSharedPtr<FString>>> ComboBox =
					SNew(SComboBox<TSharedPtr<FString>>)
						.OptionsSource(&Options_CB)
						.OnGenerateWidget(this, &SMyCustomWidget::GenerateComboBoxItem)
						.OnSelectionChanged(this, &SMyCustomWidget::HandleSelectionChanged)
						[
							SNew(STextBlock)
								.Text(this, &SMyCustomWidget::GetCurrentSelectionCB)
						]
						
				]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString("Test Slider"))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(SSlider)
						.Value(0.5f)
						.MinValue(0.0f)
						.MaxValue(1.0f)
						.OnValueChanged(this, &SMyCustomWidget::HandleSliderValueChange)
				]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SListView<TSharedPtr<FString>>)
						.ListItemsSource(&Options_CB)
						.OnGenerateRow(this, &SMyCustomWidget::GeneratedListRow)
						.OnSelectionChanged(this, &SMyCustomWidget::HandleListSelectionChanged)
				]
		]

		+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(SListView<TSharedPtr<ListViewData>>)
						.ListItemsSource(&Options_LV)
						.OnGenerateRow(this, &SMyCustomWidget::GeneratedListRowLVD)
						.OnSelectionChanged(this, &SMyCustomWidget::HandleListSelectionChangedLVD)
				]
		]

	];
}

FReply SMyCustomWidget::HandleButtonClick() 
{
	DebugHeader::ScreenPrint(TEXT("Button Clicked"), FColor::Purple);
	return FReply::Handled();
}

void SMyCustomWidget::HandleTextChanged(const FText& newText)
{
	CurrentText = newText.ToString();
	DebugHeader::ScreenPrint(CurrentText, FColor::Purple);
}

void SMyCustomWidget::HandleTextCommitted(const FText& newText, ETextCommit::Type CommitType)
{
	if (CommitType == ETextCommit::OnEnter)
	{
		DebugHeader::ScreenPrint(newText.ToString(), FColor::Magenta);
	}
}

TSharedRef<SWidget> SMyCustomWidget::GenerateComboBoxItem(TSharedPtr<FString> item)
{
	return SNew(STextBlock).Text(FText::FromString(*item.Get()));
}

void SMyCustomWidget::HandleSelectionChanged(TSharedPtr<FString> NewSelection, ESelectInfo::Type SelectInfo)
{
	Selected_CB = NewSelection;
	DebugHeader::ScreenPrint(*NewSelection, FColor::Orange);
}

FText SMyCustomWidget::GetCurrentSelectionCB() const
{
	return Selected_CB.IsValid()? FText::FromString(*Selected_CB) : FText::GetEmpty();
}

void SMyCustomWidget::HandleSliderValueChange(float newValue)
{
	value = newValue;
	UE_LOG(LogTemp, Log, TEXT("new val %f"), value);
}

TSharedRef<ITableRow> SMyCustomWidget::GeneratedListRow(TSharedPtr<FString> item,const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString("-------->"))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString(*item))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString("<--------"))
				]
		];
}

void SMyCustomWidget::HandleListSelectionChanged(TSharedPtr<FString> selectedItem, ESelectInfo::Type SelectInfo)
{
	if (selectedItem.IsValid())
	{
		DebugHeader::ScreenPrint(*selectedItem, FColor::Orange);
	}
}

TSharedRef<ITableRow> SMyCustomWidget::GeneratedListRowLVD(TSharedPtr<ListViewData> item, const TSharedRef<STableViewBase>& OwnerTable)
{
	return SNew(STableRow<TSharedPtr<FString>>, OwnerTable)
		[
			SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString(*item.Get()->foo))
				]
				+ SHorizontalBox::Slot()
				[
					SNew(STextBlock).Text(FText::FromString(*item.Get()->bar))
				]
		];
}

void SMyCustomWidget::HandleListSelectionChangedLVD(TSharedPtr<ListViewData> selectedItem, ESelectInfo::Type SelectInfo)
{
	if (selectedItem.IsValid())
	{
		DebugHeader::ScreenPrint(*selectedItem.Get()->bar, FColor::Orange);
	}
}
