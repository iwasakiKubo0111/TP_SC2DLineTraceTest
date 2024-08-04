// Fill out your copyright notice in the Description page of Project Settings.


#include "SMyInputWidget.h"
#include "Widgets/Text/STextBlock.h"
#include "Framework/Application/SlateApplication.h"

void SMyInputWidget::Construct(const FArguments& InArgs)
{
    ChildSlot
        [
            SNew(STextBlock)
                .Text(FText::FromString("Press 'W' key"))
        ];

    // このウィジェットがフォーカスを受け取るように設定
    SetCanTick(true);
    //SetCanTickInViewport(true);
    //SetKeyboardFocus();
}

FReply SMyInputWidget::OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
    UE_LOG(LogTemp, Warning, TEXT("key pressed"));
    if (InKeyEvent.GetKey() == EKeys::W)
    {
        UE_LOG(LogTemp, Warning, TEXT("W key pressed"));
        return FReply::Handled();
    }

    return FReply::Unhandled();
}