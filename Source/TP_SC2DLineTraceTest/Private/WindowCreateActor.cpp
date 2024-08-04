// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowCreateActor.h"
#include "Slate.h"
#include "SlateCore.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "Components/Widget.h"
#include <SMyInputWidget.h>
#include "Widgets/SWindow.h"
#include "Framework/Application/SlateApplication.h"

// Sets default values
AWindowCreateActor::AWindowCreateActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWindowCreateActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWindowCreateActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// 終了時の処理
void AWindowCreateActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (m_WindowOverlay) {
		
		m_WindowOverlay.Get()->ClearChildren();
		m_WindowOverlay.Reset();
	}

	if (m_CreateWindow) {
		FSlateApplication::Get().RequestDestroyWindow(m_CreateWindow.ToSharedRef());
	}
}

/*
*	ウィンドウの作成
*	param : windowSize	ウィンドウのサイズ
*	param : windowPos	ウィンドウの位置
*	param : tilte		ウィンドウのタイトル
*/
void AWindowCreateActor::CreateWindow(const FVector2D& windowSize, const FVector2D& windowPos, const FText& tilte)
{
	if (GEngine && GEngine->GameViewport) {
		// ウィンドウ作成
		m_CreateWindow = SNew(SWindow)
			.Title(tilte/*LOCTEXT("CreateWindow", "Title")*/)
			.AutoCenter(EAutoCenter::None)
			.ScreenPosition(windowPos)
			.ClientSize(windowSize)
			.CreateTitleBar(true);

		m_WindowOverlay = SNew(SOverlay);


		//ウィンドウに設定する
		m_CreateWindow->SetContent(m_WindowOverlay.ToSharedRef());
		//m_CreateWindow->OnKeyDown(FOnKeyDown::CreateRaw(this, &AWindowCreateActor::HandleKeyDown));

		// 作成したウィンドウを親ウィンドウを設定して即座表示する
		//FSlateApplication::Get().AddWindowAsNativeChild(m_CreateWindow.ToSharedRef(), GEngine->GameViewport->GetWindow().ToSharedRef(), true);
		FSlateApplication::Get().AddWindow(m_CreateWindow.ToSharedRef(), true);
		FSlateApplication::Get().SetKeyboardFocus(m_CreateWindow.ToSharedRef());
		//FSlateApplication::Get().OnKeyDown
	}
}

/*
*	ウィジットの設定
*	param : setWidget 追加するウィジット
*/
void AWindowCreateActor::SetWidget(UWidget* setWidget)
{
	if (m_WindowOverlay) {
		// ウィットを設定するためにオーバーレイに指定されたウィジェットを設定する
		auto OVerlaySlot = m_WindowOverlay.Get()->AddSlot();
		OVerlaySlot.AttachWidget(setWidget->TakeWidget());
	}
}

FReply AWindowCreateActor::HandleKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	// キーイベントを処理
	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		// Enterキーが押された時の処理
		UE_LOG(LogTemp, Warning, TEXT("Enter key pressed!"));
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

