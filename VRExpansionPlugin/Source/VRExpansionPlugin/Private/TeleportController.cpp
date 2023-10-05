// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "VRExpansionFunctionLibrary.h"
#include "DrawDebugHelpers.h"
#include "xREAL_VRCharacter.h"
#include "NavigationSystem.h"

ATeleportController::ATeleportController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bCanEverTick = true;
    bNetLoadOnClient = false;
}

void ATeleportController::BeginPlay()
{
    Super::BeginPlay();

    APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

    TeleportCylinder->SetVisibility(false, true);

    if (OwningMotionController->IsValidLowLevel())
    {
        LaserSpline->AttachToComponent(OwningMotionController, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
        LaserBeam->AttachToComponent(OwningMotionController, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
        LaserBeamEndPoint->AttachToComponent(OwningMotionController, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
        LaserBeamEndPoint->SetRelativeScale3D(FVector(.2f, .2f, .2f));

        if (OwningMotionController->IsLocallyControlled())
        {
            EnableInput(playerController);
            AVRCharacter* vrCharacter = Cast<AVRCharacter>(OwningMotionController->GetOwner());
            if (vrCharacter)
            {
                vrCharacter->OnCharacterTeleported_Bind.AddDynamic(this, &ATeleportController::CancelTracking);
                SetOwner(vrCharacter);
            }
        }
    }

    if (playerController->IsValidLowLevel())
    {
        playerController->InputComponent->BindAction("TeleportLeft", IE_Pressed, this, &ATeleportController::ActivateTeleporter);
        playerController->InputComponent->BindAction("TeleportLeft", IE_Released, this, &ATeleportController::DisableTeleporter);
        playerController->InputComponent->BindAction("TeleportRight", IE_Pressed, this, &ATeleportController::ActivateTeleporter);
        playerController->InputComponent->BindAction("TeleportRight", IE_Released, this, &ATeleportController::DisableTeleporter);
        playerController->InputComponent->BindAction("UseHeldObjectLeft", IE_Pressed, this, &ATeleportController::StartedUseHeldObjectLeft);
        playerController->InputComponent->BindAction("UseHeldObjectRight", IE_Pressed, this, &ATeleportController::StartedUseHeldObjectRight);

    }
}

void ATeleportController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    ClearArc();
    ClearLaserBeam();
    DisableWidgetActivation();
}

void ATeleportController::SetLaserBeamActive_Implementation(bool LaserBeamActive)
{
    if (LaserBeamActive != IsLaserBeamActive)
    {
        IsLaserBeamActive = LaserBeamActive;
        if (IsLaserBeamActive)
        {
            LaserBeam->SetHiddenInGame(bUseSmoothLaser);
            LaserBeamEndPoint->SetHiddenInGame(bUseSmoothLaser);
            EuroLowPassFilter.ResetSmoothingFilter();
            CreateLaserSpline();
            if (bIsLocal)
            {
                WidgetInteraction->Activate();
            }
            ToggleTick();
        }
        else
        {
            LaserBeam->SetHiddenInGame(true);
            LaserBeamEndPoint->SetHiddenInGame(true);
            ClearLaserBeam();

            // Clearing the HitResult, might not be the right way to do it
            WidgetInteraction->SetCustomHitResult(FHitResult());
            if (bIsLocal)
            {
                DisableWidgetActivation();
            }
            ToggleTick();
        }
    }
}

void ATeleportController::ActivateTeleporter_Implementation()
{
    // Set the flag, rest of the teleportation is handled in the tick function
    IsTeleporterActive = true;

    TeleportCylinder->SetVisibility(true, true);

    // Store rotation to later compare Roll value to support Wrist-based orientation of the teleporter
    if (OwningMotionController->IsValidLowLevel())
    {
        InitialControllerRotation = OwningMotionController->GetComponentRotation();
        ToggleTick();
    }
}

void ATeleportController::DisableTeleporter_Implementation()
{
    if (IsTeleporterActive)
    {
        IsTeleporterActive = false;
        TeleportCylinder->SetVisibility(false, true);
        ArcEndPoint->SetVisibility(false);
        ToggleTick();
        ClearArc();
    }
}

void ATeleportController::TraceTeleportDestination_Implementation(bool &Success, TArray<FVector> &TracePoints, FVector &NavMeshLocation, FVector &TraceLocation, FVector CurrentTraceStart, bool HitSurface, int32 NrOfSegments, TArray<FVector>& UsedTracePoints, double ProjectNavExtends)
{
    FVector worldLocation;
    FVector forwardVector;
    GetTeleWorldLocAndForwardVector(worldLocation, forwardVector);
    FHitResult hitResult;
    FVector lastTraceDestination;
    TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes = { EObjectTypeQuery::ObjectTypeQuery1 };

    UGameplayStatics::Blueprint_PredictProjectilePath_ByObjectType(this, hitResult, TracePoints, lastTraceDestination, worldLocation, forwardVector * TeleportLaunchVelocity, true, 0.0f, objectTypes, false, TArray<AActor*>(), EDrawDebugTrace::None, 0.0f, 30.0f, 2.0f, 0.0f);

    FVector hitLocation = hitResult.Location;
    FVector projectedLocation;
    //ANavigationData* navData;
    //UNavigationQueryFilter navQueryFilter;
    bool isSuccessfulProjection = UNavigationSystemV1::K2_ProjectPointToNavigation(this, hitLocation, projectedLocation,  nullptr, nullptr, FVector(ProjectNavExtends));
    NavMeshLocation = projectedLocation;
    TraceLocation = hitLocation;
    Success = (hitLocation != projectedLocation && projectedLocation != FVector::ZeroVector && isSuccessfulProjection);
    
}

void ATeleportController::ClearArc_Implementation()
{
    for (USplineMeshComponent* mesh : SplineMeshes)
    {
        mesh->DestroyComponent();
    }
    SplineMeshes.Empty();
    ArcSpline->ClearSplinePoints();
}

void ATeleportController::UpdateArcSpline_Implementation(bool FoundValidLocation, UPARAM(ref) TArray<FVector> &SplinePoints, FVector WorldLocation, FVector ForwardVector, int32 PointDiffNum)
{
    ArcSpline->ClearSplinePoints(true);
    if (!FoundValidLocation)
    {
        // Create Small Stub line when we failed to find a teleport location
        SplinePoints.Empty();
        FVector worldLocation;
        FVector forwardVector;
        GetTeleWorldLocAndForwardVector(worldLocation, forwardVector);
        SplinePoints.Add(worldLocation);
        SplinePoints.Add(worldLocation + (forwardVector * 20.0f));

    }
    for (FVector splinePoint : SplinePoints)
    {
        ArcSpline->AddSplinePoint(splinePoint, ESplineCoordinateSpace::Local, true);
    }
    ArcSpline->SetSplinePointType(SplinePoints.Num() - 1, ESplinePointType::CurveClamped, true);
    int splinePointsLastIndex = ArcSpline->GetNumberOfSplinePoints() - 1;
    if (SplineMeshes.Num() < ArcSpline->GetNumberOfSplinePoints())
    {
        PointDiffNum = splinePointsLastIndex - SplineMeshes.Num();
        for (int i = 0; i <= PointDiffNum; i++)
        {
            // Add new cylinder mesh
           USplineMeshComponent* smc = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass()); 
           // Make sure that scene is the right attachment object
           smc->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
           smc->SetGenerateOverlapEvents(false);
           smc->SetCollisionEnabled(ECollisionEnabled::NoCollision);
           SplineMeshes.Add(smc);
        }
    }
    for (int i = 0; i < SplineMeshes.Num(); i++)
    {
        if (i < splinePointsLastIndex)
        {
            SplineMeshes[i]->SetVisibility(true);
            FVector startTangent, endTangent;
            startTangent = ArcSpline->GetTangentAtSplinePoint(i, ESplineCoordinateSpace::Local);
            endTangent = ArcSpline->GetTangentAtSplinePoint(i+1, ESplineCoordinateSpace::Local);
            SplineMeshes[i]->SetStartAndEnd(SplinePoints[i], startTangent, SplinePoints[i+1], endTangent, true);
        }
        else
        {
            SplineMeshes[i]->SetVisibility(false);
        }
    }
}

void ATeleportController::UpdateArcEndpoint_Implementation(FVector NewLocation, bool ValidLocationFound)
{
    ArcEndPoint->SetVisibility(ValidLocationFound && IsTeleporterActive);
    ArcEndPoint->SetWorldLocation(NewLocation, false, nullptr, ETeleportType::TeleportPhysics);
    Arrow->SetWorldRotation(TeleportRotation + TeleportBaseRotation);
}

void ATeleportController::GetTeleportDestination_Implementation(bool RelativeToHMD, FVector &Location, FRotator &Rotation)
{
    FVector devicePosition;
    FQuat deviceRotation;
    GEngine->XRSystem->GetCurrentPose(IXRTrackingSystem::HMDDeviceId, deviceRotation, devicePosition);
    if (RelativeToHMD)
    {
        FVector heightAgnosticPosition = FVector(devicePosition.X, devicePosition.Y, 0.0f);
        Location = LastValidTeleportLocation - TeleportRotation.RotateVector(heightAgnosticPosition);
        Rotation = TeleportRotation;
    }
    else
    {
        Location = LastValidTeleportLocation;
        Rotation = TeleportRotation;
    }
}

void ATeleportController::GetTeleWorldLocAndForwardVector_Implementation(FVector &WorldLoc, FVector &ForwardVector)
{
    WorldLoc = OwningMotionController->GetComponentLocation();
    ForwardVector = UKismetMathLibrary::GetForwardVector(RotOffset + OwningMotionController->GetComponentRotation());
}

void ATeleportController::IfOverWidget_Use_Implementation(bool bPressed, bool &WasOverWidget)
{
    if (IsLaserBeamActive)
    {
        if (WidgetInteraction->IsOverInteractableWidget() || WidgetInteraction->IsOverFocusableWidget())
        {
            if (bPressed)
            {
                WidgetInteraction->PressPointerKey(EKeys::LeftMouseButton);
            }
            else
            {
                WidgetInteraction->ReleasePointerKey(EKeys::LeftMouseButton);
            }
            WasOverWidget = true;
            return;
        }
    }
    WasOverWidget = false;
}

void ATeleportController::InitController_Implementation()
{
    if (bIsLocal)
    {
        EControllerHand hand;
        OwningMotionController->GetHandType(hand);
        switch (hand)
        {
        case EControllerHand::Left:
            WidgetInteraction->VirtualUserIndex = 0;
            WidgetInteraction->PointerIndex = 0;
            break;
        
        case EControllerHand::Right:
            WidgetInteraction->VirtualUserIndex = 0;
            WidgetInteraction->PointerIndex = 1;
            break;
        
        default:
            break;
        }
    }
}

void ATeleportController::ToggleTick_Implementation()
{
    SetActorTickEnabled(IsTeleporterActive || IsLaserBeamActive || ActorBeingThrown->IsValidLowLevel());
}

void ATeleportController::ClearLaserBeam_Implementation()
{
    for (USplineMeshComponent* mesh : LaserSplineMeshes)
    {
        mesh->DestroyComponent();
    }
    LaserSplineMeshes.Empty();
    LaserSpline->ClearSplinePoints(true);
}

void ATeleportController::CreateLaserSpline_Implementation()
{
    if (bUseSmoothLaser)
    {
        for (int i = 0; i < NumberOfLaserSplinePoints; i++)
        {
           // TODO: specify the actual spline mesh to use, maybe a variable that is set in the blueprint?
           USplineMeshComponent* smc = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass()); 
           smc->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
           smc->SetCollisionEnabled(ECollisionEnabled::NoCollision);
           smc->SetGenerateOverlapEvents(false);
           LaserSplineMeshes.Add(smc);
        }
    }
}

void ATeleportController::FilterGrabspline_Implementation(UPARAM(ref) TArray<FVector> &Locations, UPARAM(ref) FVector &Target, int32 ClosestIndex, double ClosestDist)
{
    if (Locations.Num() > 1)
    {
        for (int i = 0; i < Locations.Num(); i++)
        {
            float distance = (Locations[i] - Target).SquaredLength();
            if (distance < ClosestDist || ClosestDist == 0.0f)
            {
                ClosestDist = distance;
                ClosestIndex = i;
            }
        }

        for (int i = Locations.Num() - 1; i >= 0; i--)
        {
            if (i > ClosestIndex)
            {
                Locations.RemoveAt(i);
            }
        }

        Locations[ClosestIndex] = Target;

    }
}

void ATeleportController::UpdateLaserBeam_Implementation(double Deltatime, FVector SmoothedLoc, EDrawDebugTrace::Type DrawType)
{
    if (IsLaserBeamActive)
    {
        LaserHighlightingObject = nullptr;
        WidgetInteraction->InteractionDistance = LaserBeamMaxDistance;
        FVector teleWorldLoc;
        FVector teleForwardVector;
        GetTeleWorldLocAndForwardVector(teleWorldLoc, teleForwardVector);
        FVector laserStart = teleWorldLoc;
        FVector laserEnd = teleWorldLoc + (teleForwardVector * LaserBeamMaxDistance);
        FCollisionQueryParams collisionParams;
        collisionParams.AddIgnoredActors(TArray<AActor*>({this, OwningMotionController->GetOwner()}));
        bool successfulCollision = GetWorld()->LineTraceSingleByChannel(LastLaserHitResult, laserStart, laserEnd, ECC_Visibility, collisionParams, FCollisionResponseParams::DefaultResponseParam);

        // Smooth Laser Beam
        if (bUseSmoothLaser)
        {
            // Might need to recalculate the TeleWorldLocAndForwardVector here
            FVector smoothedValue = EuroLowPassFilter.RunFilterSmoothing(teleWorldLoc + (LastLaserHitResult.Time * LaserBeamMaxDistance * teleForwardVector), Deltatime);
            UVRExpansionFunctionLibrary::SmoothUpdateLaserSpline(LaserSpline, LaserSplineMeshes, teleWorldLoc, smoothedValue, teleForwardVector, LaserBeamRadius);
            FHitResult hitResult;
            FVector smoothLineTraceEnd = smoothedValue + ((smoothedValue - teleWorldLoc).Normalize() * 100.0f);
            FCollisionQueryParams smoothCollisionParams;
            smoothCollisionParams.AddIgnoredActors(TArray<AActor*>({this, OwningMotionController->GetOwner()}));

            if (DrawSmoothLaserTrace)
            {
                DrawDebugLine(GetWorld(), teleWorldLoc, smoothLineTraceEnd, FColor::Red, false, .01f, 0, 1.0f);
            }

            if (GetWorld()->LineTraceSingleByChannel(hitResult, teleWorldLoc, smoothLineTraceEnd, ECollisionChannel::ECC_Visibility, smoothCollisionParams, FCollisionResponseParams::DefaultResponseParam))
            {
                LastLaserHitResult = hitResult;
                WidgetInteraction->SetCustomHitResult(LastLaserHitResult);
                LaserHighlightingObject = hitResult.GetComponent();
            }

            else
            {
                WidgetInteraction->SetCustomHitResult(hitResult);
            }

        }

        // Normal Laser Beam
        else
        {
            FVector scale = FVector(LastLaserHitResult.Time * LaserBeamMaxDistance, 1.0f, 1.0f);
            LaserBeam->SetWorldScale3D(scale);
            if (successfulCollision)
            {
                WidgetInteraction->SetCustomHitResult(LastLaserHitResult);
                LaserBeamEndPoint->SetRelativeLocation(FVector(LastLaserHitResult.Time * LaserBeamMaxDistance, 0.0f, 0.0f));
                LaserBeamEndPoint->SetHiddenInGame(false);
            }
            else
            {
                LaserBeamEndPoint->SetHiddenInGame(true);
            }
        }

    }
}

void ATeleportController::DisableWidgetActivation_Implementation()
{
    WidgetInteraction->SetCustomHitResult(FHitResult());
    // There was a delay here in the blueprint, could cause problems?
    WidgetInteraction->Deactivate();
}

void ATeleportController::RumbleController_Implementation(float Intensity)
{
    if (OwningMotionController->IsValidLowLevel())
    {
        EControllerHand hand;
        OwningMotionController->GetHandType(hand);
        GetWorld()->GetFirstPlayerController()->PlayHapticEffect(TeleportHapticEffect, hand, Intensity);
    }
}

 void ATeleportController::StartedUseHeldObjectLeft_Implementation()
 {
    EControllerHand hand;
    OwningMotionController->GetHandType(hand);
    if (hand == EControllerHand::Left)
    {
        TossToHand();
    }
 }

 void ATeleportController::StartedUseHeldObjectRight_Implementation()
 {
    EControllerHand hand;
    OwningMotionController->GetHandType(hand);
    if (!(hand == EControllerHand::Left))
    {
        TossToHand();
    }
 }


void ATeleportController::TossToHand_Implementation()
{
    if (IsLaserBeamActive)
    {
        //UseHeldObjectDispatch.Broadcast(LaserHighlightingObject, );

        bool isThrowing, isOverWidget;
        PhysicsTossManager->IsThrowing(isThrowing);
        isOverWidget = WidgetInteraction->IsOverInteractableWidget() || WidgetInteraction->IsOverFocusableWidget();

        if (!isThrowing && !isOverWidget && LaserHighlightingObject->IsValidLowLevel())
        {
            AxREAL_VRCharacter* vrCharacter = Cast<AxREAL_VRCharacter>(OwningMotionController->GetOwner());
            if (vrCharacter->IsValidLowLevel())
            {
                EControllerHand hand;
                OwningMotionController->GetHandType(hand);
                //TODO: Implement the following function in the AxREAL_VRCharacter class
                //vrCharacter->NotifyServerOfTossRequest(hand == EControllerHand::Left, LaserHighlightingObject);
            }
        }
    }
}

void ATeleportController::CancelTracking_Implementation()
{
    PhysicsTossManager->CancelToss();
}