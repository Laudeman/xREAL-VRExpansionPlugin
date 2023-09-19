// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"

ATeleportController::ATeleportController(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bStartWithTickEnabled = false;
    PrimaryActorTick.bCanEverTick = true;
    bNetLoadOnClient = false;
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

    if (SplineMeshes.Num() < ArcSpline->GetNumberOfSplinePoints())
    {
        PointDiffNum = (ArcSpline->GetNumberOfSplinePoints() - 1) - SplineMeshes.Num();
        for (int i = 0; i <= PointDiffNum)
        {
            
        }
    }
}

void ATeleportController::UpdateArcEndpoint_Implementation(FVector NewLocation, bool ValidLocationFound)
{

}

void ATeleportController::GetTeleportDestination_Implementation(bool RelativeToHMD, FVector &Location, FRotator &Rotation)
{
}

void ATeleportController::GetTeleWorldLocAndForwardVector_Implementation(FVector &WorldLoc, FVector &ForwardVector)
{
}

void ATeleportController::IfOverWidget_Use_Implementation(bool bPressed, bool &WasOverWidget)
{
}

void ATeleportController::InitController_Implementation()
{
}

void ATeleportController::ToggleTick_Implementation()
{
}

void ATeleportController::ClearLaserBeam_Implementation()
{
}

void ATeleportController::CreateLaserSpline_Implementation()
{
}

void ATeleportController::FilterGrabspline_Implementation(UPARAM(ref) TArray<FVector> &Locations, UPARAM(ref) FVector &Target, int32 ClosestIndex, double ClosestDist)
{
}

void ATeleportController::UpdateLaserBeam_Implementation(double Deltatime, FVector SmoothedLoc, EDrawDebugTrace::Type DrawType)
{
}

void ATeleportController::DisableWidgetActivation_Implementation()
{
    WidgetInteraction->SetCustomHitResult(FHitResult());
    // There was a delay here in the blueprint, could cause problems?
    WidgetInteraction->Deactivate();
}