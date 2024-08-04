// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowCreate.h"
#include "Slate.h"
#include "SlateCore.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "Components/Widget.h"


// 終了時の処理
UWindowCreate::~UWindowCreate()
{

	if (m_WindowOverlay) {
		// ここ重要。この処理をしないとベージコレクションされるまでリセットされないアクタのスレートポインタを保持している。というエラーが出る
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
void UWindowCreate::CreateWindow(const FVector2D& windowSize, const FVector2D& windowPos, const FText& tilte)
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

		// 作成したウィンドウを親ウィンドウを設定して即座表示する
		FSlateApplication::Get().AddWindowAsNativeChild(m_CreateWindow.ToSharedRef(), GEngine->GameViewport->GetWindow().ToSharedRef(), true);
		FSlateApplication::Get().SetKeyboardFocus(m_CreateWindow.ToSharedRef());
	}
}

/*
*	ウィジットの設定
*	param : setWidget 追加するウィジット
*/
void UWindowCreate::SetWidget(UWidget* setWidget)
{
	if (m_WindowOverlay) {
		// ウィットを設定するためにオーバーレイに指定されたウィジェットを設定する
		auto OVerlaySlot = m_WindowOverlay.Get()->AddSlot();
		OVerlaySlot.AttachWidget(setWidget->TakeWidget());
	}
}