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

	// �E�B���h�E�쐬
	UFUNCTION(BlueprintCallable, Category = "WindowCreate")
	 void CreateWindow(const FVector2D& windowSize, const FVector2D& windowPos, const FText& tilte);
	// �E�B�W�b�g�ǉ�
	UFUNCTION(BlueprintCallable, Category = "WindowCreate")
	 void SetWidget(class UWidget* setWidget);

private:
	// �E�B���h�E���
	 TSharedPtr<class SWindow> m_CreateWindow;
	// �ʂō쐬����ꂽ�E�B�W�F�b�g�z�u�p
	 TSharedPtr<class SOverlay> m_WindowOverlay;
};
