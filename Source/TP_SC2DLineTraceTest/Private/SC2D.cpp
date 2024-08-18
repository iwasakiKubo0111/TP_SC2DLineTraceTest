// Fill out your copyright notice in the Description page of Project Settings.


#include "SC2D.h"
#include "EngineUtils.h" // TActorIterator����`����Ă���
#include "Engine/TextureRenderTarget2D.h"
#include <Components/SceneCaptureComponent2D.h>
#include "Components/CanvasPanelSlot.h"
#include <Kismet/GameplayStatics.h>

/*
* �@SceneCaputure2D��Image���2D�X�N���[�����W����3D���W��ԍ��W�̌v�Z�����{��LineTrace����уA�N�^���UI��\������T���v���N���X
* 
*   �d�v�AWBP_SC2D��SceneCapture2D��[m_image]�̂悤�Ɏg�p����image�́uSize To Content�v��L�����A
* �@�@�@�@�܂��傫���̒�����Transform�z���́uScale�v�Őݒ肷��
*         �v���C���[�ɒǏ]����UI�̈ʒu�����������ꍇ��126�A127�s�ڂ̒l�𒲐�����
*/


USC2D::USC2D(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    //�v���C���[�ɒǏ]������UI�̎擾
    static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/SC2DLineTraceTest/WBP_FollowUI.WBP_FollowUI_C"));
    if (WidgetClassFinder.Succeeded())
    {
        m_tempFollowUI = WidgetClassFinder.Class;
    }
}

void USC2D::NativeConstruct()
{
	Super::NativeConstruct();

    //�Ȃ��Ă��ǂ���������Ȃ�
    SetKeyboardFocus();

	//BP_UICaptureActor���擾
    UClass* captureActorClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/SC2DLineTraceTest/BP_UICaptureActor.BP_UICaptureActor_C"));

    if (captureActorClass)
    {
        AActor* captureActor = nullptr;
        for (TActorIterator<AActor> It(GetWorld(), captureActorClass); It; ++It)
        {
            captureActor = *It;
            break; // �ŏ��̈�v����A�N�^���擾
        }

        if (captureActor)
        {
            //SceneCaptureComponent2D�������Ă��邩�m�F
            m_captureComponent = captureActor->FindComponentByClass<USceneCaptureComponent2D>();

            if (m_captureComponent)
            {
                m_renderTarget = m_captureComponent->TextureTarget;
            }

            // m_image���N���b�N�����ۂ̃C�x���g��ݒ�
            m_image->OnMouseButtonDownEvent.BindUFunction(this, FName("OnImageClicked"));
        }

    }

    //�v���C���[�ɒǏ]������UI�̐ݒ�
    if (m_tempFollowUI)
    {
        //�܂��̓v���C���[�̃A�N�^��ݒ�
        APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        APawn* playerPawn = playerController->GetPawn();
        m_actor = playerPawn;

        //�ꎞ�I�ABP_Cube���Z�b�g
        //TArray<AActor*> FoundActors;
        //UClass* BP_CubeClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/SC2DLineTraceTest/BP_Cube.BP_Cube_C"));
        //UGameplayStatics::GetAllActorsOfClass(GetWorld(), BP_CubeClass, FoundActors);
        //if (FoundActors.Num() > 0)
        //{
        //    m_actor = FoundActors[0];  // �ŏ��Ɍ��������A�N�^�[���擾
        //    // m_bpCube���g�p���鏈���������ɋL�q
        //}

        UKismetSystemLibrary::PrintString(this, TEXT("m_tempFollowUI pointer enable"), true, true, FLinearColor::Green, 2.0f);
        m_followUI = CreateWidget<UUserWidget>(GetWorld(), m_tempFollowUI);
        if (m_followUI)
        {
            UKismetSystemLibrary::PrintString(this, TEXT("newWidget pointer enable"), true, true, FLinearColor::Green, 2.0f);
            m_canvasPanel->AddChildToCanvas(m_followUI);
        }

    }


}

//�v���C���[��UI�𖈃t���[���Ǐ]�����邽�߂̏���
void USC2D::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    FVector actorPosition = m_actor->GetActorLocation();
    actorPosition.Z = (m_actor->GetComponentsBoundingBox().Max.Z) * 1.0f  ;//UI�ʒu���A�N�^�̏�����ɂ���

    FVector2D outScreenLocation;
    CaptureComponent2D_Project(m_captureComponent, actorPosition, outScreenLocation);

    //�����_�[�^�[�Q�b�g���l��
    outScreenLocation = outScreenLocation / FVector2D(m_renderTarget->SizeX, m_renderTarget->SizeY) * m_image->GetDesiredSize();

    //UKismetSystemLibrary::PrintString(this, "outScreenLocation.X:" + FString::FromInt(outScreenLocation.X) + "outScreenLocation.Y:" + FString::FromInt(outScreenLocation.Y), true, true, FLinearColor::Green, 0.5f);

    // �͈̓`�F�b�N
    FVector2D imageSize = m_image->GetDesiredSize();
    bool isInside = outScreenLocation.X > 0.1f && outScreenLocation.X <= imageSize.X &&
                    outScreenLocation.Y > 0.1f && outScreenLocation.Y <= imageSize.Y;

    if (isInside)
    {
        
        UCanvasPanelSlot* followUISlot = Cast<UCanvasPanelSlot>(m_followUI->Slot);

        FVector2D Scale = m_image->RenderTransform.Scale;
        
        //��ԉE�̌Œ�l�ɂ��āACanvas�p�l����slot�ɃZ�b�g���Ă���֌W�ł��̂܂܂��Ƃ��ꂽ�ꏊ��UI���\������Ă��܂��B���̂��ߐeCanvas��image�p�l���̍����𑫂�
        outScreenLocation.X = outScreenLocation.X * Scale.X + 40;
        outScreenLocation.Y = outScreenLocation.Y * Scale.Y + 32;
        //UKismetSystemLibrary::PrintString(this, "outScreenLocation.X:" + FString::FromInt(outScreenLocation.X) + "outScreenLocation.Y:" + FString::FromInt(outScreenLocation.Y), true, true, FLinearColor::Green, 0.5f);
        followUISlot->SetPosition(outScreenLocation);
        followUISlot->SetAlignment(FVector2D(0.5f, 0.5f));
        m_followUI->SetVisibility(ESlateVisibility::Visible);
       
    }
    else
    {
        m_followUI->SetVisibility(ESlateVisibility::Hidden);
    }   
}

//m_image���N���b�N���ꂽ�� LineTrace�����{����B(outHitResult�ɒl���ԋp�����̂ŕK�v�ɉ����Ďg�p)
FEventReply USC2D::OnImageClicked(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
    if (MouseEvent.IsMouseButtonDown(EKeys::RightMouseButton))
    {
        return FEventReply(false);
    }

    FHitResult outHitResult;
    FVector outWorldLocation;
    FVector outWorldDirection;


    FVector2D localMousePosition = MyGeometry.AbsoluteToLocal(MouseEvent.GetScreenSpacePosition());
    localMousePosition = localMousePosition * m_renderTarget->SizeX / m_image->GetDesiredSize().X;

    UKismetSystemLibrary::PrintString(this, "localMousePosition.X:" + FString::FromInt(localMousePosition.X) + "localMousePosition.Y:" + FString::FromInt(localMousePosition.Y), true, true, FLinearColor::Green, 10.0f);

    CaptureComponent2D_DeProject(m_captureComponent, localMousePosition, outWorldLocation, outWorldDirection);

    FVector EndLocation = outWorldLocation + (outWorldDirection * 10000.0f);

    TArray <AActor*> ActorIgnore;
    UKismetSystemLibrary::LineTraceSingle(GetWorld(),outWorldLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery1,false,ActorIgnore,EDrawDebugTrace::Persistent,outHitResult,true,FLinearColor::Green,FLinearColor::Red,5.0f);

    return FEventReply(true);
}

//�Q�l:https://forums.unrealengine.com/t/deproject-screen-to-world-and-scenecapture2d/91657/9
void USC2D::CaptureComponent2D_DeProject(class USceneCaptureComponent2D* Target, const FVector2D& ScreenPos, FVector& OutWorldOrigin, FVector& OutWorldDirection)
{
    if ((Target == nullptr) || (Target->TextureTarget == nullptr))
    {
        return;
    }

    const FTransform& Transform = Target->GetComponentToWorld();
    FMatrix ViewMatrix = Transform.ToInverseMatrixWithScale();
    FVector ViewLocation = Transform.GetTranslation();

    // swap axis st. x=z,y=x,z=y (unreal coord space) so that z is up
    ViewMatrix = ViewMatrix * FMatrix(
        FPlane(0, 0, 1, 0),
        FPlane(1, 0, 0, 0),
        FPlane(0, 1, 0, 0),
        FPlane(0, 0, 0, 1));

    const float FOV = Target->FOVAngle * (float)PI / 360.0f;

    FIntPoint CaptureSize(Target->TextureTarget->GetSurfaceWidth(), Target->TextureTarget->GetSurfaceHeight());

    float XAxisMultiplier;
    float YAxisMultiplier;

    if (CaptureSize.X > CaptureSize.Y)
    {
        // if the viewport is wider than it is tall
        XAxisMultiplier = 1.0f;
        YAxisMultiplier = CaptureSize.X / (float)CaptureSize.Y;
    }
    else
    {
        // if the viewport is taller than it is wide
        XAxisMultiplier = CaptureSize.Y / (float)CaptureSize.X;
        YAxisMultiplier = 1.0f;
    }

    FMatrix    ProjectionMatrix = FReversedZPerspectiveMatrix(
        FOV,
        FOV,
        XAxisMultiplier,
        YAxisMultiplier,
        GNearClippingPlane,
        GNearClippingPlane
    );

    const FMatrix InverseViewMatrix = ViewMatrix.InverseFast();
    const FMatrix InvProjectionMatrix = ProjectionMatrix.Inverse();

    const FIntRect ViewRect = FIntRect(0, 0, CaptureSize.X, CaptureSize.Y);

    FSceneView::DeprojectScreenToWorld(ScreenPos, ViewRect, InverseViewMatrix, InvProjectionMatrix, OutWorldOrigin, OutWorldDirection);
}

//�E�L�̉��p:https://forums.unrealengine.com/t/deproject-screen-to-world-and-scenecapture2d/91657/9
bool USC2D::CaptureComponent2D_Project(class USceneCaptureComponent2D* Target, const FVector& WorldLocation, FVector2D& OutScreenPos)
{
    if ((Target == nullptr) || (Target->TextureTarget == nullptr))
    {
        return false;
    }

    const FTransform& Transform = Target->GetComponentToWorld();
    FMatrix ViewMatrix = Transform.ToInverseMatrixWithScale();
    FVector ViewLocation = Transform.GetTranslation();

    // swap axis st. x=z,y=x,z=y (unreal coord space) so that z is up
    ViewMatrix = ViewMatrix * FMatrix(
        FPlane(0, 0, 1, 0),
        FPlane(1, 0, 0, 0),
        FPlane(0, 1, 0, 0),
        FPlane(0, 0, 0, 1));

    const float FOV = Target->FOVAngle * (float)PI / 360.0f;

    FIntPoint CaptureSize(Target->TextureTarget->GetSurfaceWidth(), Target->TextureTarget->GetSurfaceHeight());

    float XAxisMultiplier;
    float YAxisMultiplier;

    if (CaptureSize.X > CaptureSize.Y)
    {
        // if the viewport is wider than it is tall
        XAxisMultiplier = 1.0f;
        YAxisMultiplier = CaptureSize.X / (float)CaptureSize.Y;
    }
    else
    {
        // if the viewport is taller than it is wide
        XAxisMultiplier = CaptureSize.Y / (float)CaptureSize.X;
        YAxisMultiplier = 1.0f;
    }

    FMatrix ProjectionMatrix = FReversedZPerspectiveMatrix(
        FOV,
        FOV,
        XAxisMultiplier,
        YAxisMultiplier,
        GNearClippingPlane,
        GNearClippingPlane
    );

    const FMatrix ViewProjectionMatrix = ViewMatrix * ProjectionMatrix;
    const FIntRect ViewRect = FIntRect(0, 0, CaptureSize.X, CaptureSize.Y);

    return FSceneView::ProjectWorldToScreen(WorldLocation, ViewRect, ViewProjectionMatrix, OutScreenPos);
}





