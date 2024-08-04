// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WindowCreateActor.generated.h"

UCLASS()
class TP_SC2DLINETRACETEST_API AWindowCreateActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWindowCreateActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ウィンドウ作成
	UFUNCTION(BlueprintCallable, Category = "CreateWindowActor")
	void CreateWindow(const FVector2D& windowSize, const FVector2D& windowPos, const FText& tilte);
	// ウィジット追加
	UFUNCTION(BlueprintCallable, Category = "CreateWindowActor")
	void SetWidget(class UWidget* setWidget);

	FReply HandleKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent);

private:
	// ウィンドウ情報
	TSharedPtr<class SWindow> m_CreateWindow;
	// 別で作成されれたウィジェット配置用
	TSharedPtr<class SOverlay> m_WindowOverlay;
};