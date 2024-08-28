// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Misc/OptionalRepSkeletalMeshActor.h"
#include "Components/TimelineComponent.h"
#include "GraspingHandManny.generated.h"

// Forward declarations
class A_xREAL_VRCharacter;
class UOpenXRHandPoseComponent;
class USphereComponent;
class UVREPhysicalAnimationComponent;
class UVREPhysicsConstraintComponent;
class UCapsuleComponent;
class UTimelineComponent;
class UCurveFloat;
class UGripMotionControllerComponent;
class UAnimSequence;
class UNoRepSphereComponent;

// Enums
enum class ETriggerIndexes : uint8;
enum class EHandAnimState : uint8;
/**
 * 
 */
UCLASS()
class VREXPANSIONPLUGIN_API AGraspingHandManny : public AOptionalRepGrippableSkeletalMeshActor
{
	GENERATED_BODY()

	AGraspingHandManny(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
    virtual void BeginPlay() override;

	UFUNCTION()
	virtual void PostInitializeComponents() override;

	UFUNCTION()
    void SetupFingerOverlapBindings();

	UFUNCTION()
    void SetupAfterComponentsAreValid();

	UFUNCTION()
	UCapsuleComponent* SetupFingerCapsule(FName CapsuleName, FName SocketName, float CapsuleRadius, float CapsuleHalfHeight, FVector RelativeLocation, FVector RelativeScale, FRotator RelativeRotation = FRotator::ZeroRotator);

    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION()
    void InitPhysicsSetup();

    /** Put all finger capsule trigger information into a map */
	UFUNCTION(BlueprintCallable)
	void SetFingerCapsules();

 	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetFingerOverlapping(bool IsOverlapping, ETriggerIndexes Finger, AActor* Actor, UPrimitiveComponent* Component);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void HandleCurls(float GripCurl, ETimelineDirection::Type Direction);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void FingerMovement(ETriggerIndexes FingerIndex, float CurlSpeed, float AxisInput);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void EvaluateGrasping();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetOverlaps(bool EnableOverlaps);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetupPhysicsIfNeeded(bool SimulationOn, bool SetRelativeTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ClearFingers();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetPhysicalRelativeTrans();

	/** Sets the hand mesh to work around a quest packaging bug */
	UFUNCTION(BlueprintCallable)
	void SetMesh();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetDynamicFingerCurls(float Thumb, float Index, float Middle, float Ring, float Pinky);

	/** Returns whether or not grip is attached.  */
	UFUNCTION(BlueprintCallable, Category="Default")
	bool InitializeAndAttach(FBPActorGripInformation GripInformation, bool SecondaryGrip, bool SkipEvaluation);

	/** Returns whether or not the grip is detached. */
	UFUNCTION(BlueprintCallable)
	bool RemoveAndLerpBack(uint8 GripID, bool ForceDetach);

    UFUNCTION(BlueprintCallable)
    void ClimbingStarted(UGripMotionControllerComponent *ClimbingHand, UPrimitiveComponent *ObjectGripped, FTransform RelativeTransform);

    UFUNCTION(BlueprintCallable)
    void ClimbingEnded();

    /** Returns whether or not grip is attached.  */
	UFUNCTION(BlueprintCallable, Category="Default")
	bool InitializeAndAttachClimbing(UPrimitiveComponent* Target, FTransform RelativeTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void UpdateToNewGripTransform(bool ReCurlFingers);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void RetrievePoses(FBPActorGripInformation GripInfo);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void GetOrSpawnAttachmentProxy();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void ResetAttachmentProxy();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void BindToEvents(AxREAL_VRCharacter* OwningChar);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetupFingerAnimations();

    UFUNCTION(BlueprintCallable)
	void OnGrippedObject(UPARAM(ref) const FBPActorGripInformation& GripInfo);

    UFUNCTION(BlueprintCallable)
    void OnGripTransformChanged(UPARAM(ref) const FBPActorGripInformation& GripInfo);

    UFUNCTION(BlueprintCallable)
    void OnDroppedObject(UPARAM(ref) const FBPActorGripInformation& GripInfo, bool bWasSocketed);

	UFUNCTION(BlueprintCallable)
    void OnSocketingObject(UPARAM(ref) const FBPActorGripInformation& GripInfo, const USceneComponent *NewParentComp, FName OptionalSocketName, FTransform RelativeTransformToParent, bool WeldingBodies);

    UFUNCTION(BlueprintCallable)
	void OnLerpToHandFinished(UPARAM(ref) const FBPActorGripInformation& GripInfo);

	UFUNCTION(BlueprintCallable)
	void SecondaryAddedOnOther(UPARAM(ref) const FBPActorGripInformation& GripInfo);

	UFUNCTION(BlueprintCallable)
	void SecondaryRemovedOnOther(UPARAM(ref) const FBPActorGripInformation& GripInfo);

	UFUNCTION(BlueprintCallable)
	void FinalizeAttach(bool isClimbing);

	UFUNCTION(BlueprintCallable)
	void FinalizeDetach();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void InitGrip(FBPActorGripInformation GripInfo);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void CheckConstraintDistance();

	UFUNCTION(BlueprintCallable)
	void OnOwnerTeleported();

	UFUNCTION(BlueprintCallable)
	void OnCharacterCorrected();

	void OnTeleportIfPhysicalHand();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	bool CheckForFallbackGrip(uint8 OriginalGripID, bool WasSecondary);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable)
	void SetupClientTransforms();

	void OnTimeline_GripSmooth_Update(float Value);
	void OnTimeline_GripSmooth_Finished();

    void OnTimeline_Curl_Update(float Value);
    void OnTimeline_Curl_Finished();

	void OnTimeline_LerpBack_Update(float Value);
	void OnTimeline_LerpBack_Finished();

	// Disgusting overlap functions, plz refactor this atrocious code in some way
	UFUNCTION()
	void OnThumb3BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnThumb3EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnThumb2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnThumb2EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnIndex3BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnIndex3EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnIndex2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnIndex2EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnMiddle3BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMiddle3EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnMiddle2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnMiddle2EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnRing3BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRing3EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnRing2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnRing2EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnPinky3BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPinky3EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()
	void OnPinky2BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnPinky2EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UOpenXRHandPoseComponent> OpenXRHandPose;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USphereComponent> RootPhysics;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UVREPhysicalAnimationComponent> VREPhysicalAnimation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UVREPhysicsConstraintComponent> SimulatingHandConstraint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> pinky_02;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> ring_02;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> middle_02;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> index_02;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> thumb_02;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> thumb_03;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> ring_03;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> middle_03;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> index_03;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UCapsuleComponent> pinky_03;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Timeline")
	UCurveFloat* GripSmoothCurveFloat;

	FTimeline GripSmoothTimeline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Timeline")
	UCurveFloat* CurlCurveFloat;

	/** Lerp Finger Curls Timeline */
	FTimeline CurlTimeline;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Timeline")
	UCurveFloat* LerpBackCurveFloat;

	/** Lerp Back Timeline */
	FTimeline LerpBackTimeline;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TMap<ETriggerIndexes, UCapsuleComponent*> FingerCollisionZones;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool shouldEvaluateGrasping;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	float GraspEval_startDispl;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	float GraspEval_endDisp;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FVector GraspEval_startPoint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TMap<ETriggerIndexes, FVector> GraspEval_impactPoints;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TMap<ETriggerIndexes, float> GraspEval_Errors;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", Replicated, meta=(MultiLine="true", ExposeOnSpawn="true"))
	TObjectPtr<UGripMotionControllerComponent> OwningController;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TMap<ETriggerIndexes, float> FingerFlex;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TMap<ETriggerIndexes, bool> FingersOverlapping;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TMap<ETriggerIndexes, bool> FingersBlocked;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool HadCurled;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", Replicated, meta=(MultiLine="true"))
	FTransform BaseRelativeTransform;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool bAlreadyGrasped;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	uint8 GraspID;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FTransform BeginLerpTransform;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TObjectPtr<UObject> GrippedObject;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="AnimBP", meta=(MultiLine="true"))
	EHandAnimState HandAnimState;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", Replicated, meta=(MultiLine="true", ExposeOnSpawn="true"))
	TObjectPtr<UPrimitiveComponent> PhysicsRoot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", Replicated, meta=(MultiLine="true"))
	bool IsPhysicalHand;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", Replicated, meta=(MultiLine="true"))
	FTransform OriginalGripTrans;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FName BoneName;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	EControllerHand Laterality;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", Replicated, meta=(MultiLine="true", ExposeOnSpawn="true"))
	TObjectPtr<UGripMotionControllerComponent> OtherController;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", Replicated, meta=(MultiLine="true", ExposeOnSpawn="true"))
	bool UseCurls;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	TObjectPtr<UAnimSequence> CustomAnimation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool IsClimbing;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool bIsSecondaryGrip;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool bAllowLerpingPhysicalHandsWhenClimbing;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="HandSocket", meta=(MultiLine="true"))
	bool HasCustomAnimation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", Replicated, meta=(MultiLine="true"))
	FTransform OriginalPivotTrans;

	/** Used to work around an engine bug until it is fixed */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", Replicated, meta=(MultiLine="true"))
	TObjectPtr<UNoRepSphereComponent> AttachmentProxy;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FPoseSnapshot CustomSnapShot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="HandSocket", meta=(MultiLine="true"))
	bool CustomAnimIsSnapShot;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="HandSocket", meta=(MultiLine="true"))
	FTransform TargetMeshTransform;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="HandSocket", meta=(MultiLine="true"))
	bool bUseTargetMeshTransform;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", meta=(MultiLine="true"))
	bool HasInitializedConstraint;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", meta=(MultiLine="true"))
	FTransform Frame1;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", meta=(MultiLine="true"))
	FTransform Frame2;

	UPROPERTY()
    FTimerHandle ValidityCheckTimerHandle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", meta=(MultiLine="true"))
	FTimerHandle ConstraintCheck_Handle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", meta=(MultiLine="true"))
	double ConstraintCheckRate;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", meta=(MultiLine="true"))
	double MaxConstraintDistance;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	uint8 SecondaryGraspID;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="PhysicsHand", meta=(MultiLine="true"))
	bool bSetupWeldedDriver;
};
