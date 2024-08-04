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


// �I�����̏���
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
*	�E�B���h�E�̍쐬
*	param : windowSize	�E�B���h�E�̃T�C�Y
*	param : windowPos	�E�B���h�E�̈ʒu
*	param : tilte		�E�B���h�E�̃^�C�g��
*/
void AWindowCreateActor::CreateWindow(const FVector2D& windowSize, const FVector2D& windowPos, const FText& tilte)
{
	if (GEngine && GEngine->GameViewport) {
		// �E�B���h�E�쐬
		m_CreateWindow = SNew(SWindow)
			.Title(tilte/*LOCTEXT("CreateWindow", "Title")*/)
			.AutoCenter(EAutoCenter::None)
			.ScreenPosition(windowPos)
			.ClientSize(windowSize)
			.CreateTitleBar(true);

		m_WindowOverlay = SNew(SOverlay);


		//�E�B���h�E�ɐݒ肷��
		m_CreateWindow->SetContent(m_WindowOverlay.ToSharedRef());
		//m_CreateWindow->OnKeyDown(FOnKeyDown::CreateRaw(this, &AWindowCreateActor::HandleKeyDown));

		// �쐬�����E�B���h�E��e�E�B���h�E��ݒ肵�đ����\������
		//FSlateApplication::Get().AddWindowAsNativeChild(m_CreateWindow.ToSharedRef(), GEngine->GameViewport->GetWindow().ToSharedRef(), true);
		FSlateApplication::Get().AddWindow(m_CreateWindow.ToSharedRef(), true);
		FSlateApplication::Get().SetKeyboardFocus(m_CreateWindow.ToSharedRef());
		//FSlateApplication::Get().OnKeyDown
	}
}

/*
*	�E�B�W�b�g�̐ݒ�
*	param : setWidget �ǉ�����E�B�W�b�g
*/
void AWindowCreateActor::SetWidget(UWidget* setWidget)
{
	if (m_WindowOverlay) {
		// �E�B�b�g��ݒ肷�邽�߂ɃI�[�o�[���C�Ɏw�肳�ꂽ�E�B�W�F�b�g��ݒ肷��
		auto OVerlaySlot = m_WindowOverlay.Get()->AddSlot();
		OVerlaySlot.AttachWidget(setWidget->TakeWidget());
	}
}

FReply AWindowCreateActor::HandleKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	// �L�[�C�x���g������
	if (InKeyEvent.GetKey() == EKeys::Enter)
	{
		// Enter�L�[�������ꂽ���̏���
		UE_LOG(LogTemp, Warning, TEXT("Enter key pressed!"));
		return FReply::Handled();
	}
	return FReply::Unhandled();
}

