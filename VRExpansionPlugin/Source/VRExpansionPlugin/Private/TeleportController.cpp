// Fill out your copyright notice in the Description page of Project Settings.


#include "TeleportController.h"

void ATeleportController::SetLaserBeamActive_Implementation(bool LaserBeamActive)
{
}

void ATeleportController::ActivateTeleporter_Implementation()
{
}

void ATeleportController::DisableTeleporter_Implementation()
{
}

void ATeleportController::TraceTeleportDestination_Implementation(bool &Success, TArray<FVector> &TracePoints, FVector &NavMeshLocation, FVector &TraceLocation, FVector CurrentTraceStart, bool HitSurface, int32 NrOfSegments, TArray<FVector>& UsedTracePoints, double ProjectNavExtends)
{
}

void ATeleportController::ClearArc_Implementation()
{
}

void ATeleportController::UpdateArcSpline_Implementation(bool FoundValidLocation, UPARAM(ref) TArray<FVector> &SplinePoints, FVector WorldLocation, FVector ForwardVector, int32 PointDiffNum)
{
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
