// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "Widgets/SWindow.h"
//#include "Widgets/SOverlay.h"
//#include "Framework/Application/SlateApplication.h"
//#include "Widgets/Input/SButton.h"
//#include "Blueprint/UserWidget.h"
//#include "UObject/ConstructorHelpers.h"
#include "Widgets/SCompoundWidget.h"

/**
 * 
 */
class TP_SC2DLINETRACETEST_API SMyInputWidget : public SCompoundWidget
{
public:
    SLATE_BEGIN_ARGS(SMyInputWidget) {}
    SLATE_END_ARGS()

    void Construct(const FArguments& InArgs);

    virtual FReply OnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
};
