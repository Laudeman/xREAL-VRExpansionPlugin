// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VRCharacter.h"
#include "Enums/EVRMovementMode.h"
#include "Enums/EGripState.h"
#include "TeleportController.h"
#include "GameplayTagContainer.h"
#include "GraspingHandManny.h"
#include "OpenXRExpansionFunctionLibrary.h"
#include "Net/UnrealNetwork.h"
#include "xREAL_VRCharacter.generated.h"

class USphereComponent;
class UNoRepSphereComponent;
class UTextRenderComponent;
class UVOIPTalker;
class UInputAction;

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class VREXPANSIONPLUGIN_API AxREAL_VRCharacter : public AVRCharacter
{
	GENERATED_BODY()
public:

	AxREAL_VRCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	bool IsALocalGrip(EGripMovementReplicationSettings GripRepType);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Utility")
	void WriteToLog(bool Left, FString& Text);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void TryToGrabObject(UObject* ObjectToTryToGrab, FTransform WorldTransform, UGripMotionControllerComponent* Hand, UGripMotionControllerComponent* OtherHand, bool IsSlotGrip, FGameplayTag GripSecondaryTag, FName GripBoneName, FName SlotName, bool IsSecondaryGrip, bool& Gripped);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void GetNearestOverlappingObject(UPrimitiveComponent* OverlapComponent, UGripMotionControllerComponent* Hand, FGameplayTagContainer RelevantGameplayTags, UObject*& NearestObject, bool& ImplementsInterface, FTransform& ObjectTransform, bool& CanBeClimbed, FName& BoneName, FVector& ImpactLoc, double NearestOverlap, UObject* NearestOverlappingObject, bool ImplementsVRGrip, FTransform WorldTransform, UPrimitiveComponent* HitComponent, uint8 LastGripPrio, FName NearestBoneName, FVector ImpactPoint);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Locomotion")
	void GetDPadMovementFacing(EVRMovementMode MovementMode, UGripMotionControllerComponent* Hand, UGripMotionControllerComponent* OtherHand, FVector& ForwardVector, FVector& RightVector);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void GripOrDropObject(UGripMotionControllerComponent* CallingMotionController, UGripMotionControllerComponent* OtherController, bool CanCheckClimb, UPrimitiveComponent* GrabSphere, FGameplayTag GripTag, FGameplayTag DropTag, FGameplayTag UseTag, FGameplayTag EndUseTag, FGameplayTag GripSecondaryTag, FGameplayTag DropSecondaryTag, bool& PerformedAction, bool bHadInterface, UObject* NearestObject);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Climbing")
	void CheckAndHandleClimbingMovement(double DeltaTime);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Climbing")
	void ClearClimbing(bool BecauseOfStepUp);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Climbing")
	void InitClimbing(UGripMotionControllerComponent* Hand, UObject* Object, bool _IsObjectRelative);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CheckAndHandleGripAnimations();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void GetNearestOverlapOfHand(UGripMotionControllerComponent* Hand, UPrimitiveComponent* OverlapSphere, UObject*& NearestMesh, double NearestOverlap, UObject* NearestOverlapObject, TArray<UPrimitiveComponent*>& OverlappingComponents);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Locomotion")
	void CalculateRelativeVelocities(FVector TempVelVector);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Locomotion")
	void HandleRunInPlace(FVector ForwardVector, bool IncludeHands);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Locomotion")
	void GetSmoothedVelocityOfObject(FVector CurRelLocation, UPARAM(ref) FVector& LastRelLocation, UPARAM(ref) FVector& RelativeVelocityOut, UPARAM(ref) FVector& LowEndRelativeVelocityOut, bool bRollingAverage, FVector TempVel, FVector ABSVec);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Locomotion")
	void GetRelativeVelocityForLocomotion(bool IsHMD, bool IsMotionZVelBased, FVector VeloctyVector, double& Velocity);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CallCorrectGrabEvent(UObject* ObjectToGrip, EControllerHand Hand, bool IsSlotGrip, FTransform GripTransform, FGameplayTag GripSecondaryTag, FName OptionalBoneName, FName SlotName, bool IsSecondaryGrip);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CallCorrectDropSingleEvent(UGripMotionControllerComponent* Hand, FBPActorGripInformation Grip);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Widgets")
	void IfOverWidgetUse(UGripMotionControllerComponent* CallingHand, bool Pressed, bool& WasOverWidget);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void TryRemoveSecondaryAttachment(UGripMotionControllerComponent* CallingMotionController, UGripMotionControllerComponent* OtherController, FGameplayTagContainer GameplayTags, bool& DroppedSecondary, bool& HadSecondary);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Movement")
	void SwitchOutOfBodyCamera(bool SwitchToOutOfBody);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleport")
	void SetTeleporterActive(EControllerHand Hand, bool Active);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Locomotion")
	void HandleSlidingMovement(EVRMovementMode MovementMode, UGripMotionControllerComponent* CallingHand, bool bThumbPadInfluencesDirection);

	/**
	 * Calulates yaw rotation and magnitude from 0-1 of a -1 to 1 pad axis pair
	 * 
	 * YAxis is automatically inverted
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Locomotion")
	void CalcPadRotationAndMagnitude(float YAxis, float XAxis, float OptMagnitudeScaler, float OptionalDeadzone, FRotator& Rotation, float& Magnitude, bool& WasValid);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Teleport")
	void UpdateTeleportRotations();

	/** Pivot is in world space */
	UFUNCTION(BlueprintNativeEvent, Category="Movement|Rotation")
	void GetCharacterRotatedPosition(FVector OriginalLocation, FRotator DeltaRotation, FVector PivotPoint, FRotator& OutRotation, FVector& OutNewPosition, FRotator RotationToUse, FVector NewLocationOffset, FRotator NewRotation);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	void ValidateGameplayTag(FGameplayTag BaseTag, FGameplayTag GameplayTag, UObject* Object, FGameplayTag DefaultTag, bool& MatchedOrDefault);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Movement")
	void CycleMovementModes(bool IsLeft);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void DropItems(UGripMotionControllerComponent* Hand, FGameplayTagContainer GameplayTags);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void DropItem(UGripMotionControllerComponent* Hand, FBPActorGripInformation GripInfo, FGameplayTagContainer GameplayTags);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Misc")
	void OnDestroy();

	/**
	 * Returns socket name if is a Touch controller or object has hand specific left/right gameplay tags
	 * 
	 * Touch = VRGripTouchP
	 * 
	 * LeftTouch = VRGripTouchLP
	 * 
	 * LeftNormal = VRGripLP
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	void GetCorrectPrimarySlotPrefix(UObject* ObjectToCheckForTag, EControllerHand Hand, FName NearestBoneName, FName& SocketPrefix, bool HasPerHandSockets, FString& LocalBasePrefix);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Climbing")
	void CanObjectBeClimbed(UPrimitiveComponent* ObjectToCheck, bool& CanClimb);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	bool HasValidGripCollision(UPrimitiveComponent* Component);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Vehicle")
	void SetVehicleMode(bool IsInVehicleMode, bool& IsVR, FRotator CorrectRotation);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	bool ShouldGripComponent(UPrimitiveComponent* ComponentToCheck, uint8 GripPrioToCheckAgainst, bool bCheckAgainstPrior, FName BoneName, FGameplayTagContainer RelevantGameplayTags, UGripMotionControllerComponent* CallingController, UObject*& ObjectToGrip, bool& ObjectImplementsInterface, FTransform& ObjectsWorldTransform, uint8& GripPrio);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void TryToSecondaryGripObject(UGripMotionControllerComponent* Hand, UGripMotionControllerComponent* OtherHand, UObject* ObjectToTryToGrab, FGameplayTag GripSecondaryTag, bool ObjectImplementsInterface, FTransform RelativeSecondaryTransform, FName SlotName, bool bHadSlot, bool& SecondaryGripped);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Movement|Vel")
	void ClearMovementVelocities();

	/** Relevant GameplayTags are tags to search for and trigger operations depending on the key using this function */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void GripOrDropObjectClean(UGripMotionControllerComponent* CallingMotionController, UGripMotionControllerComponent* OtherController, bool CanCheckClimb, UPrimitiveComponent* GrabSphere, FGameplayTagContainer RelevantGameplayTags, bool& PerformedAction, bool bHadInterface, UObject* NearestObject, FName NearestBoneName, FTransform ObjectTransform, bool HadSlot, ESecondaryGripType SecondaryType, FVector ImpactPoint, FName SlotName);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	void ValidateGameplayTagContainer(FGameplayTag BaseTag, UObject* Object, FGameplayTag DefaultTag, FGameplayTagContainer GameplayTags, bool& MatchedOrDefault);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void DropSecondaryAttachment(UGripMotionControllerComponent* CallingMotionController, UGripMotionControllerComponent* OtherController, FGameplayTagContainer GameplayTags, bool& DroppedSecondary, bool& HadSecondary, bool NewLocalVar_0);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void SelectObjectFromHitArray(UPARAM(ref) TArray<FHitResult>& Hits, FGameplayTagContainer RelevantGameplayTags, UGripMotionControllerComponent* Hand, bool& bShouldGrip, bool& ObjectImplementsInterface, UObject*& ObjectToGrip, FTransform& WorldTransform, UPrimitiveComponent*& FirstPrimitiveHit, FName& BoneName, FVector& ImpactPoint, uint8 BestGripPrio, UObject* lOutObject, FTransform lOutTransform, bool lObjectImplementsInterface, bool lShouldGrip, UPrimitiveComponent* FirstHitPrimitive, FName LOutBoneName, FVector LImpactPoint);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CheckGripPriority(UObject* ObjectToCheck, uint8 PrioToCheckAgainst, bool CheckAgainstPrior, bool& HadHigherPriority, uint8& NewGripPrio);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	void GetBoneTransform(UObject* Object, FName BoneName, FTransform& BoneTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	void CanSecondaryGripObject(UGripMotionControllerComponent* Hand, UGripMotionControllerComponent* OtherHand, UObject* ObjectToTryToGrab, FGameplayTag GripSecodaryTag, bool HadSlot, ESecondaryGripType SecGripType, bool& CanSecondaryGrip);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	void CanAttemptGrabOnObject(UObject* ObjectToCheck, bool& CanAttemptGrab);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CanAttemptSecondaryGrabOnObject(UObject* ObjectToCheck, bool& CanAttemptSecondaryGrab, ESecondaryGripType& SecondaryGripType, ESecondaryGripType SecGripType);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Movement|Rotation")
	void GetCorrectRotation(FRotator& NewParam);

	/** Sets which components are used for gripping collision */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void SetGripComponents(UPrimitiveComponent* LeftHand, UPrimitiveComponent* RightHand);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping|Velocity")
	void GetThrowingVelocity(UGripMotionControllerComponent* ThrowingController, UPARAM(ref) FBPActorGripInformation& Grip, FVector AngularVel, FVector ObjectsLinearVel, FVector& angVel, FVector& LinearVelocity, FVector LocalVelocity);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void CheckSpawnGraspingHands();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Misc")
	void RepositionHandElements(bool IsRightHand, FTransform NewTransformForProcComps);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void ShouldSocketGrip(UPARAM(ref) FBPActorGripInformation& Grip, bool& ShouldSocket, USceneComponent*& SocketParent, FTransform_NetQuantize& RelativeTransform, FName& OptionalSocketName);

	/** Initialize controllers and setup voice */
	UFUNCTION(BlueprintNativeEvent)
	void InitTeleportControllers(APlayerState* ValidPlayerState, bool PlayerStateToUse);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CheckIsValidForGripping(UObject* Object, FGameplayTagContainer RelevantGameplayTags, bool& IsValidToGrip);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Gripping")
	void RemoveControllerScale(FTransform SocketTransform, UGripMotionControllerComponent* GrippingController, FTransform& FinalTransform);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CheckUseHeldItems(UGripMotionControllerComponent* Hand, bool ButtonState);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Movement")
	void GetCorrectAimComp(UGripMotionControllerComponent* Hand, USceneComponent*& AimComp);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Input")
	void MapInput();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping|Velocity")
	void SampleGripVelocities();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Gripping")
	void CheckUseSecondaryAttachment(UGripMotionControllerComponent* CallingMotionController, UGripMotionControllerComponent* OtherController, bool ButtonPressed, bool& DroppedOrUsedSecondary, bool& HadSecondary, bool NewLocalVar_0);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Movement")
	void HandleCurrentMovementInput(float MovementInput, UGripMotionControllerComponent* MovingHand, UGripMotionControllerComponent* OtherHand);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Movement")
	void HandleTurnInput(float InputAxis, float InputValue);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, Category="Movement")
	void SetMovementHands(bool RightHandForMovement);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure, Category="Locomotion")
	FVector MapThumbToWorld(FRotator PadRotation, UGripMotionControllerComponent* CallingHand);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_RightControllerOffset();

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void OnRep_LeftControllerOffset();

private:

	// Input Handler Functions
	UFUNCTION()
	void ControllerMovementRight_Handler(const FInputActionValue& Value);

	UFUNCTION()
	void ControllerMovementLeft_Handler(const FInputActionValue& Value);

	UFUNCTION()
	void MotionControllerThumbLeft_X_Handler(const FInputActionValue& Value);

	UFUNCTION()
	void MotionControllerThumbLeft_Y_Handler(const FInputActionValue& Value);

	UFUNCTION()
	void MotionControllerThumbRight_X_Handler(const FInputActionValue& Value);

	UFUNCTION()
	void MotionControllerThumbRight_Y_Handler(const FInputActionValue& Value);

public:

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	double MinimumLowEndRipVelocity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USphereComponent> GrabSphereRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UNoRepSphereComponent> AttachmentProxyLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UNoRepSphereComponent> AttachmentProxyRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> AimRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USceneComponent> AimLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UTextRenderComponent> TextL;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UVOIPTalker> VOIPTalker;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> HeadMesh;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UTextRenderComponent> TextR;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USphereComponent> GrabSphereLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USkeletalMeshComponent> HandMesh_Left;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UStaticMeshComponent> Body;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<UTextRenderComponent> PlayerNameDisplay;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc")
	FTimerHandle ControllerTimerHandle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Teleport")
	bool IsTeleporting;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Teleport")
	double FadeinDuration;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Teleport")
	FLinearColor TeleportFadeColor;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Teleport")
	double FadeOutDuration;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Teleport", Transient)
	TObjectPtr<ATeleportController> TeleportControllerLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Teleport", Transient)
	TObjectPtr<ATeleportController> TeleportControllerRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	EVRMovementMode CurrentMovementMode;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	EVRMovementMode MovementModeRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	double DPadVelocityScaler;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	double GripTraceLength;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Climbing")
	FTransform ClimbGripLocation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Climbing")
	bool IsHandClimbing;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Climbing")
	TObjectPtr<UGripMotionControllerComponent> ClimbingHand;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Climbing")
	TObjectPtr<UPrimitiveComponent> GrippedObject;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Climbing")
	bool IsObjectRelative;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Control Booleans")
	bool AlwaysAllowClimbing;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	EGripState HandStateLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	EGripState HandStateRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	double SwingAndRunMagnitude;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector LeftControllerRelativeVel;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector RightControllerRelativeVel;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector HeadRelativeVel;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	double RunningInPlaceScaler;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	int32 RIPMotionSmoothingSteps;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector LastLContPos;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector LastRContPos;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector LastHMDPos;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Control Booleans")
	bool bIsArmSwingZBased;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	double MinimumRIPVelocity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Misc")
	FTimerHandle TrackedDeviceTimerHandle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	int32 RipMotionLowPassSmoothingSteps;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector LowEndLContRelativeVel;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector LowEndRcontRelativeVel;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Vel")
	FVector LowEndHeadRelativeVel;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category="Default")
	TObjectPtr<USkeletalMeshComponent> HandMesh_Right;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Control Booleans")
	bool bIncludeHandsForRIP;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	bool bIsOutOfBody;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Control Booleans", meta=(DisplayName="B Thumb Pad Effects Sliding Direction"))
	bool bThumbPadEffectsSlidingDirection;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Control Booleans", meta=(DisplayName="B Teleport Uses Thumb Rotation"))
	bool bTeleportUsesThumbRotation;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Teleport")
	double TeleportThumbDeadzone;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping", meta=(MultiLine="true"))
	FGameplayTag DefaultGripTag;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	FGameplayTag DefaultDropTag;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	FGameplayTag DefaultSecondaryDropTag;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	FGameplayTag DefaultSecondaryGripTag;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	EBPHMDDeviceType HeadsetType;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	bool bWasAlreadyPossessed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Vehicle")
	bool bIsInVehicle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	bool DisableMovement;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default")
	TObjectPtr<ACameraActor> OutOfBodyCamera;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionToCheckDuringGrip;

	/** If true, will skip trace step of checking for components to grip */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping")
	bool bForceOverlapOnlyGripChecks;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="VOIP")
	TObjectPtr<USoundAttenuation> AttenuationSettingsForVOIP;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TObjectPtr<UPrimitiveComponent> LeftHandGripComponent;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default")
	TObjectPtr<UPrimitiveComponent> RightHandGripComponent;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement")
	double SlidingMovementDeadZone;

	/** Please add a variable description */
	static_assert(true, "You will need to add DOREPLIFETIME_WITH_PARAMS(ABP_VRCharacter, LeftControllerOffset, COND_SkipOwner) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", ReplicatedUsing="OnRep_LeftControllerOffset")
	FTransform_NetQuantize LeftControllerOffset;

	/** Please add a variable description */
	static_assert(true, "You will need to add DOREPLIFETIME_WITH_PARAMS(ABP_VRCharacter, RightControllerOffset, COND_SkipOwner) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", ReplicatedUsing="OnRep_RightControllerOffset")
	FTransform_NetQuantize RightControllerOffset;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool SpawnGraspingHands;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool UsePhysicalGraspingHands;

	/** Please add a variable description */
	static_assert(true, "You will need to add DOREPLIFETIME(ABP_VRCharacter, GraspingHandRight) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", Replicated, meta=(MultiLine="true"))
	TObjectPtr<AGraspingHandManny> GraspingHandRight;

	/** Please add a variable description */
	static_assert(true, "You will need to add DOREPLIFETIME(ABP_VRCharacter, GraspingHandLeft) to GetLifetimeReplicatedProps");
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", Replicated, meta=(MultiLine="true"))
	TObjectPtr<AGraspingHandManny> GraspingHandLeft;

	/** If true then we  will sample the controller velocity on release if we aren't sampling the objects velocity */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	bool UseControllerVelocityOnRelease;

	/** what mass we should consider full strength, anything more than this starts to scale throw speed down */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping", meta=(MultiLine="true"))
	double ThrowingMassScaleFactor;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	bool bSampleGripVelocity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	FBPLowPassPeakFilter PeakVelocityLeft;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	FBPLowPassPeakFilter PeakVelocityRight;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	bool ScaleVelocityByMass;

	/** Maximum mass at which we have full velocity, it will scale down from here up */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	double ThrowingMassMaximum;

	/** Minimum value that mass scaling can go to */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	double MassScalerMin;

	/** Maximum size of the throwing velocity, we will clamp to this */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	double MaximumThrowingVelocity;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Gripping|Velocity", meta=(MultiLine="true"))
	bool bLimitMaxThrowVelocity;

	/** Use OpenXR Hand Tracking - Used to be SteamVR Curls */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	bool GraspingHandsUseFingerCurls;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnClimbingInitiated, UGripMotionControllerComponent*, ClimbingHand, UPrimitiveComponent*, GrippedObject, FTransform, RelativeTransform);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FOnClimbingInitiated OnClimbingInitiated;

	/** Please add a variable description */
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClimbingEnded);
	UPROPERTY(BlueprintAssignable, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	FOnClimbingEnded OnClimbingEnded;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement", meta=(MultiLine="true"))
	bool bRightHandMovement;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement|Turning", meta=(MultiLine="true"))
	bool bTurnModeIsSnap;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Turning", meta=(MultiLine="true"))
	double SnapTurnAngle;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Turning", meta=(MultiLine="true"))
	double SmoothTurnSpeed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Turning", meta=(MultiLine="true", UIMin="0", UIMax="1", ClampMin="0", ClampMax="1"))
	float TurningActivationThreshold;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Movement|Turning", meta=(MultiLine="true"))
	bool bTurningFlag;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="ControllerProfile", meta=(MultiLine="true"))
	EBPOpenXRControllerDeviceType CurrentControllerTypeXR;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Input")
	TObjectPtr<UPlayerMappableInputConfig> InputConfig;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Movement")
	bool bTwoHandMovement;

private:

	UPROPERTY()
	float MotionControllerThumbLeft_X_Value;

	UPROPERTY()
	float MotionControllerThumbLeft_Y_Value;

	UPROPERTY()
	float MotionControllerThumbRight_X_Value;

	UPROPERTY()
	float MotionControllerThumbRight_Y_Value;

	//Blueprint Input Actions
    UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MotionControllerThumbLeft_X;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MotionControllerThumbLeft_Y;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MotionControllerThumbRight_X;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MotionControllerThumbRight_Y;
	
};
