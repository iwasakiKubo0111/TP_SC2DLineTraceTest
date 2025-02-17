// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Engine/SceneCapture2D.h"
#include <Components/CanvasPanel.h>
#include "SC2D.generated.h"

/**
 * 
 */
UCLASS()
class TP_SC2DLINETRACETEST_API USC2D : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USC2D(const FObjectInitializer& ObjectInitializer);

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	USceneCaptureComponent2D* m_captureComponent;

	UFUNCTION(BlueprintCallable, Category = "USC2D")
	FEventReply OnImageClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent);

	void CaptureComponent2D_DeProject(USceneCaptureComponent2D* Target, const FVector2D& ScreenPos, FVector& OutWorldOrigin, FVector& OutWorldDirection);

	bool CaptureComponent2D_Project(USceneCaptureComponent2D* Target, const FVector& WorldLocation, FVector2D& OutScreenPos);

private:

	/*UPROPERTY(EditAnywhere, Category = "USC2D")
	TSubclassOf<AActor> BP_CubeClass;*/

	UPROPERTY(meta = (BindWidget))
	UImage* m_image;

	UTextureRenderTarget2D* m_renderTarget;

	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* m_canvasPanel;

	TSubclassOf<UUserWidget> m_tempFollowUI;
	UUserWidget* m_followUI;

	AActor* m_actor;
};
