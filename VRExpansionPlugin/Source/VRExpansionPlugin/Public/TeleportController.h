// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysicsTossManager.h"
#include "Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "Enums/EGripState.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Haptics/HapticFeedbackEffect_Base.h"
#include "TeleportController.generated.h"

UCLASS(Blueprintable, BlueprintType)
class VREXPANSIONPLUGIN_API ATeleportController : public AActor
{
	GENERATED_BODY()
	public:

	ATeleportController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Laser")
	void SetLaserBeamActive(bool LaserBeamActive);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleportation")
	void ActivateTeleporter();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleportation")
	void DisableTeleporter();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleportation")
	void TraceTeleportDestination(bool& Success, TArray<FVector>& TracePoints, FVector& NavMeshLocation, FVector& TraceLocation, FVector CurrentTraceStart, bool HitSurface, int32 NrOfSegments, TArray<FVector>& UsedTracePoints, double ProjectNavExtends);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleportation")
	void ClearArc();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleportation")
	void UpdateArcSpline(bool FoundValidLocation, UPARAM(ref) TArray<FVector>& SplinePoints, FVector WorldLocation, FVector ForwardVector, int32 PointDiffNum);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleportation")
	void UpdateArcEndpoint(FVector NewLocation, bool ValidLocationFound);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Teleportation")
	void GetTeleportDestination(bool RelativeToHMD, FVector& Location, FRotator& Rotation);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Teleportation")
	void GetTeleWorldLocAndForwardVector(FVector& WorldLoc, FVector& ForwardVector);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void IfOverWidget_Use(bool bPressed, bool& WasOverWidget);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void InitController();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void ToggleTick();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Default")
	void ClearLaserBeam();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Laser")
	void CreateLaserSpline();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void FilterGrabspline(UPARAM(ref) TArray<FVector>& Locations, UPARAM(ref) FVector& Target, int32 ClosestIndex, double ClosestDist);

	UFUNCTION(BlueprintNativeEvent)
	void DisableWidgetActivation();

	UFUNCTION(BlueprintNativeEvent)
	void RumbleController(float Intensity);

	UFUNCTION(BlueprintNativeEvent)
	void StartedUseHeldObjectLeft();

	UFUNCTION(BlueprintNativeEvent)
	void StartedUseHeldObjectRight();

	UFUNCTION(BlueprintNativeEvent)
	void TossToHand();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CancelTracking();


protected:
	/** TODO: Change this back to a private function, had to switch it to protected in order to use it as BlueprintNativeEvent.*/
	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Laser")
	void UpdateLaserBeam(double Deltatime, FVector SmoothedLoc, EDrawDebugTrace::Type DrawType);
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	double TeleportLaunchVelocity;

	// Haptic feedback to play when teleportation is activated
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<UHapticFeedbackEffect_Base> TeleportHapticEffect;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USplineComponent> LaserSpline;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> LaserBeamEndPoint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> FinalFacingArrow;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UWidgetInteractionComponent> WidgetInteraction;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USplineMeshComponent> LaserBeam;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> Arrow;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USplineComponent> ArcSpline;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> Ring;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> TeleportCylinder;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> ArcEndPoint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> Scene;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<AActor> AttachedActor;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool IsTeleporterActive;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TArray<USplineMeshComponent*> SplineMeshes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool bLastFrameValidDestination;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool IsRoomScale;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool IsValidTeleportDestination;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TEnumAsByte<EGripState> GripState;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool bWantsToGrip;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FRotator TeleportRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UPhysicsTossManager> PhysicsTossManager;

	/** Rotation of Motion Controller at the start of the Teleport press. */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FRotator InitialControllerRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(ExposeOnSpawn="true"))
	TObjectPtr<UGripMotionControllerComponent> OwningMotionController;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool IsLaserBeamActive;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	double LaserBeamMaxDistance;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(ExposeOnSpawn="true"))
	bool bIsLocal;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool bArcDirty;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FRotator TeleportBaseRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	FVector LastValidTeleportLocation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Laser")
	FBPEuroLowPassFilter EuroLowPassFilter;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Laser")
	TArray<USplineMeshComponent*> LaserSplineMeshes;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Laser")
	int32 NumberOfLaserSplinePoints;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Laser")
	double LaserBeamRadius;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Laser", meta=(DisplayName="B Use Smooth Laser"))
	bool bUseSmoothLaser;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Laser")
	bool DrawSmoothLaserTrace;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Laser")
	FHitResult LastLaserHitResult;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool bShowTrackingSensors;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TObjectPtr<UPrimitiveComponent> LaserHighlightingObject;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TObjectPtr<UPrimitiveComponent> ActorBeingThrown;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(MultiLine="true"))
	FRotator RotOffset;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUseHeldObjectDispatch, UPrimitiveComponent*, HighlightedObject, bool, LeftHand);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Default")
	FUseHeldObjectDispatch UseHeldObjectDispatch;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNotUseHeldObjectDispatch);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Default")
	FNotUseHeldObjectDispatch NotUseHeldObjectDispatch;


};
