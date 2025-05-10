// Fill out your copyright notice in the Description page of Project Settings.


#include "SlateWidgets/MyCustomWidget.h"
void SMyCustomWidget::Construct(const FArguments& inArgs)
{
	bCanSupportFocus = true;

	inArgs._TestString;
	FSlateFontInfo TitleFont = FCoreStyle::Get().GetFontStyle(FName("EmbossedText"));
	TitleFont.Size = 30;
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

	];
}
