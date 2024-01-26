// Fill out your copyright notice in the Description page of Project Settings.


#include "xREAL_VRCharacter.h"
#include "Components/TextRenderComponent.h"

AxREAL_VRCharacter::AxREAL_VRCharacter(const FObjectInitializer& ObjectInitializer)
	: Super() {
	//: Super(ObjectInitializer.SetDefaultSubobjectClass<UVRRootComponent>(ACharacter::CapsuleComponentName).SetDefaultSubobjectClass<UVRCharacterMovementComponent>(ACharacter::CharacterMovementComponentName)) {
    bReplicates = true;
}

bool AxREAL_VRCharacter::IsALocalGrip_Implementation(EGripMovementReplicationSettings GripRepType)
{
    if (GripRepType == EGripMovementReplicationSettings::ClientSide_Authoritive || GripRepType == EGripMovementReplicationSettings::ClientSide_Authoritive_NoRep)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void AxREAL_VRCharacter::WriteToLog_Implementation(bool Left, FString &Text)
{
    if (Left)
    {
        TextL->SetText(FText::FromString(Text));
    }
    else
    {
        TextR->SetText(FText::FromString(Text));
    }
}

void AxREAL_VRCharacter::TryToGrabObject_Implementation(UObject *ObjectToTryToGrab, FTransform WorldTransform, UGripMotionControllerComponent *Hand, UGripMotionControllerComponent *OtherHand, bool IsSlotGrip, FGameplayTag GripSecondaryTag, FName GripBoneName, FName SlotName, bool IsSecondaryGrip, bool &Gripped)
{
    bool implementsInterface = ObjectToTryToGrab->GetClass()->ImplementsInterface(UVRGripInterface::StaticClass());

    if (Hand->GetIsObjectHeld(ObjectToTryToGrab))
    {
        Gripped = false;
        return;
    }
    else
    {
        if (OtherHand->GetIsObjectHeld(ObjectToTryToGrab))
        {
            //Cast object to grip interface
        }
    }
}

void AxREAL_VRCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> & OutLifetimeProps) const {
    DOREPLIFETIME(AxREAL_VRCharacter, LeftControllerOffset);
    DOREPLIFETIME(AxREAL_VRCharacter, RightControllerOffset);
    DOREPLIFETIME(AxREAL_VRCharacter, GraspingHandLeft);
    DOREPLIFETIME(AxREAL_VRCharacter, GraspingHandRight);
}

void AxREAL_VRCharacter::GetNearestOverlappingObject_Implementation(UPrimitiveComponent *OverlapComponent, UGripMotionControllerComponent *Hand, FGameplayTagContainer RelevantGameplayTags, UObject *&NearestObject, bool &ImplementsInterface, FTransform &ObjectTransform, bool &CanBeClimbed, FName &BoneName, FVector &ImpactLoc, double NearestOverlap, UObject *NearestOverlappingObject, bool ImplementsVRGrip, FTransform WorldTransform, UPrimitiveComponent *HitComponent, uint8 LastGripPrio, FName NearestBoneName, FVector ImpactPoint)
{
}

void AxREAL_VRCharacter::GetDPadMovementFacing_Implementation(EVRMovementMode MovementMode, UGripMotionControllerComponent *Hand, UGripMotionControllerComponent *OtherHand, FVector &ForwardVector, FVector &RightVector)
{
}

void AxREAL_VRCharacter::GripOrDropObject_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, bool CanCheckClimb, UPrimitiveComponent *GrabSphere, FGameplayTag GripTag, FGameplayTag DropTag, FGameplayTag UseTag, FGameplayTag EndUseTag, FGameplayTag GripSecondaryTag, FGameplayTag DropSecondaryTag, bool &PerformedAction, bool bHadInterface, UObject *NearestObject)
{
}

void AxREAL_VRCharacter::CheckAndHandleClimbingMovement_Implementation(double DeltaTime)
{
}

void AxREAL_VRCharacter::ClearClimbing_Implementation(bool BecauseOfStepUp)
{
}

void AxREAL_VRCharacter::InitClimbing_Implementation(UGripMotionControllerComponent *Hand, UObject *Object, bool _IsObjectRelative)
{
}

void AxREAL_VRCharacter::CheckAndHandleGripAnimations_Implementation()
{
}

void AxREAL_VRCharacter::GetNearestOverlapOfHand_Implementation(UGripMotionControllerComponent *Hand, UPrimitiveComponent *OverlapSphere, UObject *&NearestMesh, double NearestOverlap, UObject *NearestOverlapObject, TArray<UPrimitiveComponent *>& OverlappingComponents)
{
}

void AxREAL_VRCharacter::CalculateRelativeVelocities_Implementation(FVector TempVelVector)
{
}

void AxREAL_VRCharacter::HandleRunInPlace_Implementation(FVector ForwardVector, bool IncludeHands)
{
}

void AxREAL_VRCharacter::GetSmoothedVelocityOfObject_Implementation(FVector CurRelLocation, UPARAM(ref) FVector &LastRelLocation, UPARAM(ref) FVector &RelativeVelocityOut, UPARAM(ref) FVector &LowEndRelativeVelocityOut, bool bRollingAverage, FVector TempVel, FVector ABSVec)
{
}

void AxREAL_VRCharacter::GetRelativeVelocityForLocomotion_Implementation(bool IsHMD, bool IsMotionZVelBased, FVector VeloctyVector, double &Velocity)
{
}

void AxREAL_VRCharacter::CallCorrectGrabEvent_Implementation(UObject *ObjectToGrip, EControllerHand Hand, bool IsSlotGrip, FTransform GripTransform, FGameplayTag GripSecondaryTag, FName OptionalBoneName, FName SlotName, bool IsSecondaryGrip)
{
}

void AxREAL_VRCharacter::CallCorrectDropSingleEvent_Implementation(UGripMotionControllerComponent *Hand, FBPActorGripInformation Grip)
{
}

void AxREAL_VRCharacter::IfOverWidgetUse_Implementation(UGripMotionControllerComponent *CallingHand, bool Pressed, bool &WasOverWidget)
{
}

void AxREAL_VRCharacter::TryRemoveSecondaryAttachment_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, FGameplayTagContainer GameplayTags, bool &DroppedSecondary, bool &HadSecondary)
{
}

void AxREAL_VRCharacter::SwitchOutOfBodyCamera_Implementation(bool SwitchToOutOfBody)
{
}

void AxREAL_VRCharacter::SetTeleporterActive_Implementation(EControllerHand Hand, bool Active)
{
    switch (Hand)
    {
    case EControllerHand::Left:
        if (TeleportControllerLeft->IsValidLowLevel()) 
        {
            
        }
        break;
    
    default:
        break;
    }
}

void AxREAL_VRCharacter::HandleSlidingMovement_Implementation(EVRMovementMode MovementMode, UGripMotionControllerComponent *CallingHand, bool bThumbPadInfluencesDirection, double ThumbY, double ThumbX, FVector Direction)
{
}

void AxREAL_VRCharacter::CalcPadRotationAndMagnitude_Implementation(double YAxis, double XAxis, double OptMagnitudeScaler, double OptionalDeadzone, FRotator &Rotation, double &Magnitude, bool &WasValid)
{
}

void AxREAL_VRCharacter::UpdateTeleportRotations_Implementation()
{
}

void AxREAL_VRCharacter::GetCharacterRotatedPosition_Implementation(FVector OriginalLocation, FRotator DeltaRotation, FVector PivotPoint, FRotator &OutRotation, FVector &OutNewPosition, FRotator RotationToUse, FVector NewLocationOffset, FRotator NewRotation)
{
}

void AxREAL_VRCharacter::ValidateGameplayTag_Implementation(FGameplayTag BaseTag, FGameplayTag GameplayTag, UObject *Object, FGameplayTag DefaultTag, bool &MatchedOrDefault)
{
}

void AxREAL_VRCharacter::CycleMovementModes_Implementation(bool IsLeft)
{
}

void AxREAL_VRCharacter::DropItems_Implementation(UGripMotionControllerComponent *Hand, FGameplayTagContainer GameplayTags)
{
}

void AxREAL_VRCharacter::DropItem_Implementation(UGripMotionControllerComponent *Hand, FBPActorGripInformation GripInfo, FGameplayTagContainer GameplayTags)
{
}

void AxREAL_VRCharacter::OnDestroy_Implementation()
{
}

void AxREAL_VRCharacter::GetCorrectPrimarySlotPrefix_Implementation(UObject *ObjectToCheckForTag, EControllerHand Hand, FName NearestBoneName, FName &SocketPrefix, bool HasPerHandSockets, FString& LocalBasePrefix)
{
}

void AxREAL_VRCharacter::CanObjectBeClimbed_Implementation(UPrimitiveComponent *ObjectToCheck, bool &CanClimb)
{
}

void AxREAL_VRCharacter::HasValidGripCollision_Implementation(UPrimitiveComponent *Component, bool &IsValid)
{
}

void AxREAL_VRCharacter::SetVehicleMode_Implementation(bool IsInVehicleMode, bool &IsVR, FRotator CorrectRotation)
{
}

void AxREAL_VRCharacter::ShouldGripComponent_Implementation(UPrimitiveComponent *ComponentToCheck, uint8 GripPrioToCheckAgainst, bool bCheckAgainstPrior, FName BoneName, FGameplayTagContainer RelevantGameplayTags, UGripMotionControllerComponent *CallingController, bool &ShouldGrip, UObject *&ObjectToGrip, bool &ObjectImplementsInterface, FTransform &ObjectsWorldTransform, uint8 &GripPrio, AActor *OwningActor, bool ImplementsInterface)
{
}

void AxREAL_VRCharacter::TryToSecondaryGripObject_Implementation(UGripMotionControllerComponent *Hand, UGripMotionControllerComponent *OtherHand, UObject *ObjectToTryToGrab, FGameplayTag GripSecondaryTag, bool ObjectImplementsInterface, FTransform RelativeSecondaryTransform, FName SlotName, bool bHadSlot, bool &SecondaryGripped, ESecondaryGripType SecondaryGripType)
{
}

void AxREAL_VRCharacter::ClearMovementVelocities_Implementation()
{
}

void AxREAL_VRCharacter::GripOrDropObjectClean_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, bool CanCheckClimb, UPrimitiveComponent *GrabSphere, FGameplayTagContainer RelevantGameplayTags, bool &PerformedAction, bool bHadInterface, UObject *NearestObject, FName NearestBoneName, FTransform ObjectTransform, bool HadSlot, ESecondaryGripType SecondaryType, FVector ImpactPoint, FName SlotName)
{
}

void AxREAL_VRCharacter::ValidateGameplayTagContainer_Implementation(FGameplayTag BaseTag, UObject *Object, FGameplayTag DefaultTag, FGameplayTagContainer GameplayTags, bool &MatchedOrDefault)
{
}

void AxREAL_VRCharacter::DropSecondaryAttachment_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, FGameplayTagContainer GameplayTags, bool &DroppedSecondary, bool &HadSecondary, bool NewLocalVar_0)
{
}

void AxREAL_VRCharacter::SelectObjectFromHitArray_Implementation(UPARAM(ref) TArray<FHitResult> &Hits, FGameplayTagContainer RelevantGameplayTags, UGripMotionControllerComponent *Hand, bool &bShouldGrip, bool &ObjectImplementsInterface, UObject *&ObjectToGrip, FTransform &WorldTransform, UPrimitiveComponent *&FirstPrimitiveHit, FName &BoneName, FVector &ImpactPoint, uint8 BestGripPrio, UObject *lOutObject, FTransform lOutTransform, bool lObjectImplementsInterface, bool lShouldGrip, UPrimitiveComponent *FirstHitPrimitive, FName LOutBoneName, FVector LImpactPoint)
{
}

void AxREAL_VRCharacter::CheckGripPriority_Implementation(UObject *ObjectToCheck, uint8 PrioToCheckAgainst, bool CheckAgainstPrior, bool &HadHigherPriority, uint8 &NewGripPrio)
{
}

void AxREAL_VRCharacter::GetBoneTransform_Implementation(UObject *Object, FName BoneName, FTransform &BoneTransform)
{
}

void AxREAL_VRCharacter::CanSecondaryGripObject_Implementation(UGripMotionControllerComponent *Hand, UGripMotionControllerComponent *OtherHand, UObject *ObjectToTryToGrab, FGameplayTag GripSecodaryTag, bool HadSlot, ESecondaryGripType SecGripType, bool &CanSecondaryGrip)
{
}

void AxREAL_VRCharacter::CanAttemptGrabOnObject_Implementation(UObject *ObjectToCheck, bool &CanAttemptGrab)
{
}

void AxREAL_VRCharacter::CanAttemptSecondaryGrabOnObject_Implementation(UObject *ObjectToCheck, bool &CanAttemptSecondaryGrab, ESecondaryGripType &SecondaryGripType, ESecondaryGripType SecGripType)
{
}

void AxREAL_VRCharacter::GetCorrectRotation_Implementation(FRotator &NewParam)
{
}

void AxREAL_VRCharacter::SetGripComponents_Implementation(UPrimitiveComponent *LeftHand, UPrimitiveComponent *RightHand)
{
}

void AxREAL_VRCharacter::GetThrowingVelocity_Implementation(UGripMotionControllerComponent *ThrowingController, UPARAM(ref) FBPActorGripInformation &Grip, FVector AngularVel, FVector ObjectsLinearVel, FVector &angVel, FVector &LinearVelocity, FVector LocalVelocity)
{
}

void AxREAL_VRCharacter::CheckSpawnGraspingHands_Implementation()
{
}

void AxREAL_VRCharacter::RepositionHandElements_Implementation(bool IsRightHand, FTransform NewTransformForProcComps)
{
}

void AxREAL_VRCharacter::ShouldSocketGrip_Implementation(UPARAM(ref) FBPActorGripInformation &Grip, bool &ShouldSocket, USceneComponent *&SocketParent, FTransform_NetQuantize &RelativeTransform, FName &OptionalSocketName)
{
}

void AxREAL_VRCharacter::InitTeleportControllers_Implementation(APlayerState *ValidPlayerState, bool PlayerStateToUse)
{
}

void AxREAL_VRCharacter::CheckIsValidForGripping_Implementation(UObject *Object, FGameplayTagContainer RelevantGameplayTags, bool &IsValidToGrip)
{
}

void AxREAL_VRCharacter::RemoveControllerScale_Implementation(FTransform SocketTransform, UGripMotionControllerComponent *GrippingController, FTransform &FinalTransform)
{
}

void AxREAL_VRCharacter::CheckUseHeldItems_Implementation(UGripMotionControllerComponent *Hand, bool ButtonState)
{
}

void AxREAL_VRCharacter::GetCorrectAimComp_Implementation(UGripMotionControllerComponent *Hand, USceneComponent *&AimComp)
{
}

void AxREAL_VRCharacter::MapInput_Implementation()
{
}

void AxREAL_VRCharacter::SampleGripVelocities_Implementation()
{
}

void AxREAL_VRCharacter::CheckUseSecondaryAttachment_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, bool ButtonPressed, bool &DroppedOrUsedSecondary, bool &HadSecondary, bool NewLocalVar_0)
{
}

void AxREAL_VRCharacter::HandleCurrentMovementInput_Implementation(double MovementInput, UGripMotionControllerComponent *MovingHand, UGripMotionControllerComponent *OtherHand)
{
}

void AxREAL_VRCharacter::HandleTurnInput_Implementation(double InputAxis, double InputValue)
{
}

void AxREAL_VRCharacter::SetMovementHands_Implementation(bool RightHandForMovement)
{
}

void AxREAL_VRCharacter::MapThumbToWorld_Implementation(FRotator PadRotation, UGripMotionControllerComponent *CallingHand, FVector &Direction)
{
}

void AxREAL_VRCharacter::OnRep_RightControllerOffset_Implementation()
{
}

void AxREAL_VRCharacter::OnRep_LeftControllerOffset_Implementation()
{
}
