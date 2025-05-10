// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/MyCustomWidget.h"
#include "../DebugHeader.h"
void SMyCustomWidget::Construct(const FArguments& inArgs)
{
	bCanSupportFocus = true;

	inArgs._TestString;
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleFont.Size = 30;
	Options_CB.Add(MakeShared<FString>("foo"));
	Options_CB.Add(MakeShared<FString>("bar"));
	Options_CB.Add(MakeShared<FString>("fuzz"));
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
					SNew(STextBlock).Text(FText::FromString("Test CheckBox"))
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
