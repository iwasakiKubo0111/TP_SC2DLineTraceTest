// Fill out your copyright notice in the Description page of Project Settings.


#include "WindowCreate.h"
#include "Slate.h"
#include "SlateCore.h"
#include "Engine/GameViewportClient.h"
#include "Engine/Engine.h"
#include "Components/Widget.h"


// �I�����̏���
UWindowCreate::~UWindowCreate()
{

	if (m_WindowOverlay) {
		// �����d�v�B���̏��������Ȃ��ƃx�[�W�R���N�V���������܂Ń��Z�b�g����Ȃ��A�N�^�̃X���[�g�|�C���^��ێ����Ă���B�Ƃ����G���[���o��
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
void UWindowCreate::CreateWindow(const FVector2D& windowSize, const FVector2D& windowPos, const FText& tilte)
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

		// �쐬�����E�B���h�E��e�E�B���h�E��ݒ肵�đ����\������
		FSlateApplication::Get().AddWindowAsNativeChild(m_CreateWindow.ToSharedRef(), GEngine->GameViewport->GetWindow().ToSharedRef(), true);
		FSlateApplication::Get().SetKeyboardFocus(m_CreateWindow.ToSharedRef());
	}
}

/*
*	�E�B�W�b�g�̐ݒ�
*	param : setWidget �ǉ�����E�B�W�b�g
*/
void UWindowCreate::SetWidget(UWidget* setWidget)
{
	if (m_WindowOverlay) {
		// �E�B�b�g��ݒ肷�邽�߂ɃI�[�o�[���C�Ɏw�肳�ꂽ�E�B�W�F�b�g��ݒ肷��
		auto OVerlaySlot = m_WindowOverlay.Get()->AddSlot();
		OVerlaySlot.AttachWidget(setWidget->TakeWidget());
	}
}