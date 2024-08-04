// Fill out your copyright notice in the Description page of Project Settings.


#include "SC2D.h"
#include "EngineUtils.h" // TActorIteratorが定義されている
#include "Engine/TextureRenderTarget2D.h"
#include <Components/SceneCaptureComponent2D.h>
#include <Kismet/GameplayStatics.h>

USC2D::USC2D(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    static ConstructorHelpers::FClassFinder<AActor> BP_Cube(TEXT("/Game/SC2DLineTraceTest/BP_Cube.BP_Cube_C"));
    if (BP_Cube.Succeeded())
    {
        BP_CubeClass = BP_Cube.Class;
    }

    //SetKeyboardFocus();
}

void USC2D::NativeConstruct()
{
	Super::NativeConstruct();

    //bIsFocusable
    SetKeyboardFocus();

	//BP_UICaptureActorを取得
    UClass* captureActorClass = StaticLoadClass(AActor::StaticClass(), nullptr, TEXT("/Game/SC2DLineTraceTest/BP_UICaptureActor.BP_UICaptureActor_C"));

    if (captureActorClass)
    {
        AActor* captureActor = nullptr;
        for (TActorIterator<AActor> It(GetWorld(), captureActorClass); It; ++It)
        {
            captureActor = *It;
            break; // 最初の一致するアクタを取得
        }

        if (captureActor)
        {
            //SceneCaptureComponent2Dを持っているか確認
            m_captureComponent = captureActor->FindComponentByClass<USceneCaptureComponent2D>();

            if (m_captureComponent)
            {
                m_renderTarget = m_captureComponent->TextureTarget;
                //captureComponent->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
            }

            // UMGウィジェットのセットアップ
            if (m_image)
            {
                UKismetSystemLibrary::PrintString(this, TEXT("m_image pointer enable"), true, true, FLinearColor::Green, 2.0f);
            }

            // クリックイベントの処理を設定
            m_image->OnMouseButtonDownEvent.BindUFunction(this, FName("OnImageClicked"));
            //m_image->OnMouseButtonDownEvent.BindUFunction(this, "OnImageClicked");


        }

    }

}


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

    CaptureComponent2D_DeProject(m_captureComponent, localMousePosition, outWorldLocation, outWorldDirection);

    FVector EndLocation = outWorldLocation + (outWorldDirection * 10000.0f);

    TArray <AActor*> ActorIgnore;
    UKismetSystemLibrary::LineTraceSingle(GetWorld(),outWorldLocation,EndLocation,ETraceTypeQuery::TraceTypeQuery1,false,ActorIgnore,EDrawDebugTrace::Persistent,outHitResult,true,FLinearColor::Green,FLinearColor::Red,5.0f);

    return FEventReply(true);
}

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





