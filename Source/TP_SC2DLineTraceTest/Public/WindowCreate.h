// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WindowCreate.generated.h"

/**
 * 
 */
UCLASS()
class TP_SC2DLINETRACETEST_API UWindowCreate : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	~UWindowCreate();

	// ウィンドウ作成
	UFUNCTION(BlueprintCallable, Category = "WindowCreate")
	 void CreateWindow(const FVector2D& windowSize, const FVector2D& windowPos, const FText& tilte);
	// ウィジット追加
	UFUNCTION(BlueprintCallable, Category = "WindowCreate")
	 void SetWidget(class UWidget* setWidget);

private:
	// ウィンドウ情報
	 TSharedPtr<class SWindow> m_CreateWindow;
	// 別で作成されれたウィジェット配置用
	 TSharedPtr<class SOverlay> m_WindowOverlay;
};
