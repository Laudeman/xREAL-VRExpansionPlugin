// Fill out your copyright notice in the Description page of Project Settings.


#include "xREAL_VRCharacter.h"
#include "Components/TextRenderComponent.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagAssetInterface.h"
#include "VRExpansionFunctionLibrary.h"
#include "Camera/CameraActor.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AxREAL_VRCharacter::AxREAL_VRCharacter(const FObjectInitializer& ObjectInitializer): Super() 
{
    bReplicates = true;
}

void AxREAL_VRCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) 
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* PlayerEnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        MotionControllerThumbLeft_X = FindObject<UInputAction>(ANY_PACKAGE, TEXT("MotionControllerThumbLeft_X"));
        if (MotionControllerThumbLeft_X)
        {
            PlayerEnhancedInputComponent->BindAction(MotionControllerThumbLeft_X, ETriggerEvent::Triggered, this, &AxREAL_VRCharacter::MotionControllerThumbLeft_X_Handler);
        }
        MotionControllerThumbLeft_Y = FindObject<UInputAction>(ANY_PACKAGE, TEXT("MotionControllerThumbLeft_Y"));
        if (MotionControllerThumbLeft_Y)
        {
            PlayerEnhancedInputComponent->BindAction(MotionControllerThumbLeft_Y, ETriggerEvent::Triggered, this, &AxREAL_VRCharacter::MotionControllerThumbLeft_Y_Handler);
        }
        MotionControllerThumbRight_X = FindObject<UInputAction>(ANY_PACKAGE, TEXT("MotionControllerThumbRight_X"));
        if (MotionControllerThumbRight_X)
        {
            PlayerEnhancedInputComponent->BindAction(MotionControllerThumbRight_X, ETriggerEvent::Triggered, this, &AxREAL_VRCharacter::MotionControllerThumbRight_X_Handler);
        }
        MotionControllerThumbRight_Y = FindObject<UInputAction>(ANY_PACKAGE, TEXT("MotionControllerThumbRight_Y"));
        if (MotionControllerThumbRight_Y)
        {
            PlayerEnhancedInputComponent->BindAction(MotionControllerThumbRight_Y, ETriggerEvent::Triggered, this, &AxREAL_VRCharacter::MotionControllerThumbRight_Y_Handler);
        }
    }
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
            IVRGripInterface *objectToTryToGrab = Cast<IVRGripInterface>(ObjectToTryToGrab);
            if (objectToTryToGrab && objectToTryToGrab->AllowsMultipleGrips())
            {
                
            }
            else
            {
                if (IsSecondaryGrip || !IsSlotGrip)
                {
                    bool isSecondaryGripped;
                    TryToSecondaryGripObject(Hand, OtherHand, ObjectToTryToGrab, GripSecondaryTag, implementsInterface, WorldTransform, SlotName, IsSlotGrip, isSecondaryGripped);
                    if (isSecondaryGripped)
                    {
                        Gripped = true;
                        return;
                    }
                }
                OtherHand->DropObject(ObjectToTryToGrab, 0);
            }

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
    if (OverlapComponent->IsValidLowLevel())
    {
        if (bForceOverlapOnlyGripChecks)
        {
            FallBackOnOverlap(OverlapComponent, HitComponent, Hand, RelevantGameplayTags, NearestObject, NearestOverlappingObject, ImplementsVRGrip, WorldTransform, CanBeClimbed, NearestBoneName, ImpactPoint, LastGripPrio);
        }
        else
        {
            TArray<FHitResult> outHits;
            bool bHasHits = PerformTraceForObjects(OverlapComponent, Hand, outHits);
            if (bHasHits)
            {
                // Can we grip one of the components, and filter by priority
                bool shouldGrip, objectImplementsInterface;
                UObject* objectToGrip;
                FTransform worldTransform;
                UPrimitiveComponent* firstPrimitiveHit;
                FName boneName;
                FVector impactPoint;
                SelectObjectFromHitArray(outHits, RelevantGameplayTags, Hand, shouldGrip, objectImplementsInterface, objectToGrip, worldTransform, firstPrimitiveHit, boneName, impactPoint);
                if (shouldGrip)
                {
                    NearestOverlappingObject = objectToGrip;
                    ImplementsVRGrip = objectImplementsInterface;
                    WorldTransform = worldTransform;
                    ImpactPoint = impactPoint;
                    //TODO: Check if this works
                    if (IGameplayTagAssetInterface* GameplayTagAsset = Cast<IGameplayTagAssetInterface>(objectToGrip))
                    {
                        bool hasTag = GameplayTagAsset->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("Interactible.PerBoneGripping")));
                        if (hasTag)
                        {
                            NearestBoneName = BoneName;
                        }
                        else
                        {
                            NearestBoneName = "None";
                        }
                    }
                }
                else
                {
                    HitComponent = firstPrimitiveHit;
                    if (!NearestOverlappingObject->IsValidLowLevel())
                    {
                    // Return climbable if not simulating and not grippable
                    
                        if (IsClimbingModeEnabled(Hand) && CanObjectBeClimbed(HitComponent))
                        {
                            CanBeClimbed = true;
                            NearestObject = HitComponent;
                        }
                    }
                }
            }
            else
            {
                FallBackOnOverlap(OverlapComponent, HitComponent, Hand, RelevantGameplayTags, NearestObject, NearestOverlappingObject, ImplementsVRGrip, WorldTransform, CanBeClimbed, NearestBoneName, ImpactPoint, LastGripPrio);
            }
        }

        // Return results
        NearestObject = NearestOverlappingObject;
        ImplementsInterface = ImplementsVRGrip;
        ObjectTransform = WorldTransform;
        CanBeClimbed = false;
        BoneName = NearestBoneName;
        ImpactLoc = ImpactPoint;
        return;
    }
    else
    {
        GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("GripComponent is not valid"));
        NearestObject = nullptr;
    }
}

bool AxREAL_VRCharacter::IsClimbingModeEnabled(UGripMotionControllerComponent *Hand)
{
    EControllerHand handType;
    Hand->GetHandType(handType);
    return ((handType == EControllerHand::Left && (CurrentMovementMode == EVRMovementMode::ClimbingMode || AlwaysAllowClimbing)) ||
            (handType == EControllerHand::Right && (MovementModeRight == EVRMovementMode::ClimbingMode || AlwaysAllowClimbing)));
}

bool AxREAL_VRCharacter::PerformTraceForObjects(UPrimitiveComponent* OverlapComponent, UGripMotionControllerComponent* Hand, TArray<FHitResult>& OutHits)
{
    // Try to Trace for the object first - Preferred
    FBoxSphereBounds overlapComponentBounds = OverlapComponent->Bounds;
    FVector overlapComponentRotationVector = OverlapComponent->GetComponentRotation().Vector();
    FVector gripVector = overlapComponentRotationVector * GripTraceLength;
    FVector startTracePoint = overlapComponentBounds.Origin - gripVector;
    FVector endTracePoint = overlapComponentBounds.Origin + gripVector;

    float radius = overlapComponentBounds.SphereRadius;
    TArray<AActor*> actorsToIgnore;
    Hand->GetGrippedActors(actorsToIgnore);

    FCollisionQueryParams queryParams = FCollisionQueryParams(FName(TEXT("GripOverlap")), true);
    queryParams.AddIgnoredActors(actorsToIgnore);
    ECollisionChannel vrTraceChannel = ECollisionChannel::ECC_GameTraceChannel1; //VR Trace Channel
    return GetWorld()->SweepMultiByChannel(OutHits, startTracePoint, endTracePoint, FQuat::Identity, vrTraceChannel, \
    FCollisionShape::MakeSphere(radius), queryParams);
}

void AxREAL_VRCharacter::FallBackOnOverlap(UPrimitiveComponent *&OverlapComponent, UPrimitiveComponent *&HitComponent, UGripMotionControllerComponent *&Hand, FGameplayTagContainer RelevantGameplayTags, UObject *&NearestObject, UObject *&NearestOverlappingObject, bool &ImplementsVRGrip, FTransform &WorldTransform, bool &CanBeClimbed, FName &NearestBoneName, FVector &ImpactPoint, uint8 &LastGripPrio)
{
    FTransform overlapTransform = OverlapComponent->GetComponentTransform();
    TArray<AActor*> actorsToIgnore = {this};
    TArray<UPrimitiveComponent*> overlappingComponents;
    // Fall back on overlap, being inside of an object can cause this.
    bool bHasOverlaps = UKismetSystemLibrary::ComponentOverlapComponents(OverlapComponent, overlapTransform, CollisionToCheckDuringGrip, nullptr, actorsToIgnore, overlappingComponents);
    if (bHasOverlaps)
    {
        for (int i = 0; i < overlappingComponents.Num(); i++)
        {
            if (i == 0)
            {
                HitComponent = overlappingComponents[i];
            }
            if (HasValidGripCollision(overlappingComponents[i]))
            {
                UObject* nearestOverlappingObject;
                bool implementsVRGrip;
                FTransform worldTransform;
                uint8 lastGripPrio;
                if (ShouldGripComponent(overlappingComponents[i], LastGripPrio, i > 0, "None", RelevantGameplayTags, Hand, nearestOverlappingObject, implementsVRGrip, worldTransform, lastGripPrio))
                {
                    NearestOverlappingObject = nearestOverlappingObject;
                    ImplementsVRGrip = implementsVRGrip;
                    WorldTransform = worldTransform;
                    ImpactPoint = OverlapComponent->GetComponentLocation();
                    LastGripPrio = lastGripPrio;
                }
            }
        }
        if (!NearestOverlappingObject->IsValidLowLevel())
        {
            // Return climbable if not simulating and not grippable
            
            if (IsClimbingModeEnabled(Hand) && CanObjectBeClimbed(HitComponent))
            {
                CanBeClimbed = true;
                NearestObject = HitComponent;
            }
        }
    }
}
void AxREAL_VRCharacter::GetDPadMovementFacing_Implementation(EVRMovementMode MovementMode, UGripMotionControllerComponent *Hand, UGripMotionControllerComponent *OtherHand, FVector &ForwardVector, FVector &RightVector)
{
    USceneComponent* handAimComp;
    USceneComponent* otherHandAimComp;
    switch (MovementMode)
    {
        case EVRMovementMode::Teleport:
            ForwardVector = FVector::ZeroVector;
            RightVector = FVector::ZeroVector;
            break;

        case EVRMovementMode::Navigate:
            ForwardVector = FVector::ZeroVector;
            RightVector = FVector::ZeroVector;
            break;

        case EVRMovementMode::DPadPress_ControllerOrient:
            handAimComp = GetCorrectAimComp(Hand);
            if (handAimComp)
            {
                ForwardVector = FVector::VectorPlaneProject(handAimComp->GetForwardVector(), GetVRUpVector()).GetSafeNormal(0.0001f);
                RightVector = FVector::VectorPlaneProject(handAimComp->GetRightVector(), GetVRUpVector()).GetSafeNormal(0.0001f);
            }
            break;
        
        case EVRMovementMode::DPadPress_HMDOrient:
            ForwardVector = GetVRForwardVector();
            RightVector = GetVRRightVector();
            break;

        case EVRMovementMode::RunInPlace:
            otherHandAimComp = GetCorrectAimComp(OtherHand);
            handAimComp = GetCorrectAimComp(Hand);
            if (otherHandAimComp && handAimComp)
            {
                FVector otherHandForwardVector = FVector::VectorPlaneProject(otherHandAimComp->GetForwardVector(), GetVRUpVector()).GetSafeNormal(0.0001f);
                FVector handForwardVector = FVector::VectorPlaneProject(handAimComp->GetForwardVector(), GetVRUpVector()).GetSafeNormal(0.0001f);

                ForwardVector = (otherHandForwardVector + handForwardVector) / 2.0f;

                FVector otherHandRightVector = FVector::VectorPlaneProject(otherHandAimComp->GetRightVector(), GetVRUpVector()).GetSafeNormal(0.0001f);
                FVector handRightVector = FVector::VectorPlaneProject(handAimComp->GetRightVector(), GetVRUpVector()).GetSafeNormal(0.0001f);

                RightVector = (otherHandRightVector + handRightVector) / 2.0f;
            }
            break;

        case EVRMovementMode::RunInPlaceHeadForward:
            ForwardVector = GetVRForwardVector();
            RightVector = GetVRRightVector();
            break;
        
        case EVRMovementMode::ArmSwing:
            otherHandAimComp = GetCorrectAimComp(OtherHand);
            handAimComp = GetCorrectAimComp(Hand);
            if (otherHandAimComp && handAimComp)
            {
                FVector otherHandForwardVector = FVector::VectorPlaneProject(otherHandAimComp->GetForwardVector(), GetVRUpVector()).GetSafeNormal(0.0001f);
                FVector handForwardVector = FVector::VectorPlaneProject(handAimComp->GetForwardVector(), GetVRUpVector()).GetSafeNormal(0.0001f);

                ForwardVector = (otherHandForwardVector + handForwardVector) / 2.0f;

                FVector otherHandRightVector = FVector::VectorPlaneProject(otherHandAimComp->GetRightVector(), GetVRUpVector()).GetSafeNormal(0.0001f);
                FVector handRightVector = FVector::VectorPlaneProject(handAimComp->GetRightVector(), GetVRUpVector()).GetSafeNormal(0.0001f);

                RightVector = (otherHandRightVector + handRightVector) / 2.0f;
            }
            break;

        case EVRMovementMode::ClimbingMode:
            ForwardVector = FVector::ZeroVector;
            RightVector = FVector::ZeroVector;
            break;
        
        default:
            break;
    }
}

void AxREAL_VRCharacter::GripOrDropObject_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, bool CanCheckClimb, UPrimitiveComponent *GrabSphere, FGameplayTag GripTag, FGameplayTag DropTag, FGameplayTag UseTag, FGameplayTag EndUseTag, FGameplayTag GripSecondaryTag, FGameplayTag DropSecondaryTag, bool &PerformedAction)
{
    TArray<FGameplayTag> relevantTags = {GripTag, DropTag, UseTag, EndUseTag, GripSecondaryTag, DropSecondaryTag};
    FGameplayTagContainer relevantGameplayTagContainer = FGameplayTagContainer::CreateFromArray(relevantTags);
    GripOrDropObjectClean(CallingMotionController, OtherController, CanCheckClimb, GrabSphere, relevantGameplayTagContainer, PerformedAction);
}

void AxREAL_VRCharacter::CheckAndHandleClimbingMovement_Implementation(double DeltaTime)
{
    if (IsLocallyControlled() && IsHandClimbing && ClimbingHand->IsValidLowLevel())
    {
        // Object Relative Climbing
        if (IsObjectRelative)
        {
            if (GrippedObject->IsValidLowLevel())
            {
                FVector movement = UKismetMathLibrary::NegateVector(ClimbingHand->GetPivotLocation() - UKismetMathLibrary::TransformLocation(GrippedObject->GetComponentTransform(), ClimbGripLocation.GetLocation()));
                VRMovementReference->AddCustomReplicatedMovement(movement);
            }
        }
        // World Position Climbing
        else
        {
            FVector movement = UKismetMathLibrary::NegateVector(ClimbingHand->GetPivotLocation() - ClimbGripLocation.GetLocation());
            VRMovementReference->AddCustomReplicatedMovement(movement);
        }
    }
}

void AxREAL_VRCharacter::ClearClimbing_Implementation(bool BecauseOfStepUp)
{
    if (IsHandClimbing && IsLocallyControlled())
    {
        if (BecauseOfStepUp)
        {
            IsHandClimbing = false;
            ClimbingHand = nullptr;
            ClimbGripLocation = FTransform::Identity;
            IsObjectRelative = false;
            GrippedObject = nullptr;
            OnClimbingEnded.Broadcast();
        }
    }
}

void AxREAL_VRCharacter::InitClimbing_Implementation(UGripMotionControllerComponent *Hand, UObject *Object, bool _IsObjectRelative)
{
    if (IsHandClimbing && ClimbingHand != Hand)
    {
        OnClimbingEnded.Broadcast();
    }
    UPrimitiveComponent* objectAsPrimitive = Cast<UPrimitiveComponent>(Object);
    if (objectAsPrimitive)
    {
        FTransform objectTransform = objectAsPrimitive->GetComponentTransform();
        ClimbGripLocation = UKismetMathLibrary::MakeRelativeTransform(Hand->GetPivotTransform(), objectTransform);
        GrippedObject = objectAsPrimitive;
        IsObjectRelative = _IsObjectRelative;
    }
    else
    {
        ClimbGripLocation = Hand->GetPivotTransform();
        IsObjectRelative = false;
    }
    IsHandClimbing = true;
    ClimbingHand = Hand;
    VRMovementReference->SetClimbingMode(true);
    OnClimbingInitiated.Broadcast(ClimbingHand, GrippedObject, ClimbGripLocation);
}

void AxREAL_VRCharacter::CheckAndHandleGripAnimations_Implementation()
{
    // Left Hand
    if (LeftHandGripComponent->IsValidLowLevel() && !HandMesh_Left->bOwnerNoSee)
    {
        if (LeftMotionController->HasGrippedObjects())
        {
            HandStateLeft = EGripState::Grab;
        }
        else
        {
            // Ensure something was found
            if (GetNearestOverlapOfHand(LeftMotionController, LeftHandGripComponent) != nullptr)
            {
                HandStateLeft = EGripState::CanGrab;
            }
            else
            {
                HandStateLeft = EGripState::Open;
            }
        }
        //HandMesh_Left->GetAnimInstance()
    }
    if (RightHandGripComponent->IsValidLowLevel() && !HandMesh_Right->bOwnerNoSee)
    {
        if (RightMotionController->HasGrippedObjects())
        {
            HandStateRight = EGripState::Grab;
        }
        else
        {
            // Ensure something was found
            if (GetNearestOverlapOfHand(RightMotionController, RightHandGripComponent) != nullptr)
            {
                HandStateRight = EGripState::CanGrab;
            }
            else
            {
                HandStateRight = EGripState::Open;
            }
        }
    }
}

UObject* AxREAL_VRCharacter::GetNearestOverlapOfHand_Implementation(UGripMotionControllerComponent *Hand, UPrimitiveComponent *OverlapSphere)
{
    TArray<UPrimitiveComponent*> overlappingComponents;
    OverlapSphere->GetOverlappingComponents(overlappingComponents);

    float nearestOverlap = 1000000.0f;
    UObject* nearestOverlapObject = nullptr;

    for (UPrimitiveComponent* component : overlappingComponents)
    {
        float distanceSquared = (component->GetComponentLocation() - OverlapSphere->GetComponentLocation()).SizeSquared();
        if (distanceSquared < nearestOverlap)
        {
            nearestOverlapObject = component;
            nearestOverlap = distanceSquared; // For some reason this was set to distance not squared in the Blueprint
            //nearestOverlap = (component->GetComponentLocation() - OverlapSphere->GetComponentLocation()).Size();
        }
        /*
        else
        {
            return nearestOverlapObject;
        }*/
    }
    return nearestOverlapObject;
}

void AxREAL_VRCharacter::CalculateRelativeVelocities_Implementation(FVector TempVelVector)
{
    if (CurrentMovementMode >= EVRMovementMode::RunInPlace)
    {
        //Smooths over VelCalculationsSmoothingSteps
        GetSmoothedVelocityOfObject(LeftMotionController->GetRelativeLocation(), LastLContPos, LeftControllerRelativeVel, LowEndLContRelativeVel, true);
        GetSmoothedVelocityOfObject(RightMotionController->GetRelativeLocation(), LastRContPos, RightControllerRelativeVel, LowEndRcontRelativeVel, true);
        GetSmoothedVelocityOfObject(VRReplicatedCamera->GetRelativeLocation(), LastHMDPos, HeadRelativeVel, LowEndHeadRelativeVel, true);
    }
}

void AxREAL_VRCharacter::HandleRunInPlace_Implementation(FVector ForwardVector, bool IncludeHands)
{
    float headRelativeVelocity = GetRelativeVelocityForLocomotion(true, bIsArmSwingZBased, HeadRelativeVel);
    float leftControllerRelativeVelocity = GetRelativeVelocityForLocomotion(true, bIsArmSwingZBased, LeftControllerRelativeVel);
    float rightControllerRelativeVelocity = GetRelativeVelocityForLocomotion(true, bIsArmSwingZBased, RightControllerRelativeVel);
    float averageVelocity = (headRelativeVelocity + leftControllerRelativeVelocity + rightControllerRelativeVelocity) / 3.0f;
    // Used to set a min scaler to cutoff earlier on stop
    float lowEndHeadRelativeVelocity = GetRelativeVelocityForLocomotion(true, bIsArmSwingZBased, LowEndHeadRelativeVel);
    if (IncludeHands)
    {
        if (averageVelocity >= MinimumRIPVelocity && lowEndHeadRelativeVelocity >= MinimumLowEndRipVelocity)
        {
            AddMovementInput(ForwardVector, UKismetMathLibrary::Clamp(RunningInPlaceScaler * averageVelocity, 0.0f, 1.0f), false);
        }
    }
    else
    {
        // Not including hands
        if (headRelativeVelocity >= MinimumRIPVelocity && lowEndHeadRelativeVelocity >= MinimumLowEndRipVelocity)
        {
            AddMovementInput(ForwardVector, UKismetMathLibrary::Clamp(RunningInPlaceScaler * headRelativeVelocity, 0.0f, 1.0f), false);
        }
    }
}

void AxREAL_VRCharacter::GetSmoothedVelocityOfObject_Implementation(FVector CurRelLocation, UPARAM(ref) FVector &LastRelLocation, UPARAM(ref) FVector &RelativeVelocityOut, UPARAM(ref) FVector &LowEndRelativeVelocityOut, bool bRollingAverage)
{
    FVector tempVel;
    FVector relativeLocationDifference = CurRelLocation - LastRelLocation;
    FVector ABSVector = FVector(FMath::Abs(relativeLocationDifference.X), FMath::Abs(relativeLocationDifference.Y), FMath::Abs(relativeLocationDifference.Z));
    if (bRollingAverage)
    {
        // New rolling average low pass
        UVRExpansionFunctionLibrary::LowPassFilter_RollingAverage(RelativeVelocityOut, ABSVector, RelativeVelocityOut, RIPMotionSmoothingSteps);
        // Low end out
        UVRExpansionFunctionLibrary::LowPassFilter_RollingAverage(RelativeVelocityOut, ABSVector, LowEndRelativeVelocityOut, RipMotionLowPassSmoothingSteps);
    }
    else
    {
        // New exponential low pass
        UVRExpansionFunctionLibrary::LowPassFilter_Exponential(RelativeVelocityOut, ABSVector, RelativeVelocityOut, 0.3f);
    }
    // Set Last rel loc
    LastRelLocation = CurRelLocation;
}

float AxREAL_VRCharacter::GetRelativeVelocityForLocomotion_Implementation(bool IsHMD, bool IsMotionZVelBased, FVector VeloctyVector)
{
    // HMD is always Z based
    if (IsHMD || IsMotionZVelBased)
    {
        return VeloctyVector.Z;
    }
    else
    {
        return VeloctyVector.Size();
    }
}

void AxREAL_VRCharacter::CallCorrectGrabEvent_Implementation(UObject *ObjectToGrip, EControllerHand Hand, bool IsSlotGrip, FTransform GripTransform, FGameplayTag GripSecondaryTag, FName OptionalBoneName, FName SlotName, bool IsSecondaryGrip)
{
    if (ObjectToGrip->Implements<UVRGripInterface>())
    {
        if (IsALocalGrip(IVRGripInterface::Execute_GripMovementReplicationType(ObjectToGrip)))
        {
            TryGrabClient(ObjectToGrip, IsSlotGrip, GripTransform, Hand, GripSecondaryTag, OptionalBoneName, SlotName, IsSecondaryGrip);
        }
        else
        {
            TryGrabServer(ObjectToGrip, IsSlotGrip, GripTransform, Hand, GripSecondaryTag, OptionalBoneName, SlotName, IsSecondaryGrip);
        }
    }
}

void AxREAL_VRCharacter::TryGrabClient_Implementation(UObject* ObjectToGrab, bool IsSlotGrip, FTransform_NetQuantize GripTransform, EControllerHand Hand, FGameplayTag GripSecondaryTag, FName GripBoneName, FName SlotName, bool IsSecondaryGrip)
{
    bool isGrabbed;
    switch (Hand)
    {
        case EControllerHand::Left:
            TryToGrabObject(ObjectToGrab, GripTransform, LeftMotionController, RightMotionController, IsSlotGrip, GripSecondaryTag, GripBoneName, SlotName, IsSecondaryGrip, isGrabbed);
            break;

        case EControllerHand::Right:
            TryToGrabObject(ObjectToGrab, GripTransform, RightMotionController, LeftMotionController, IsSlotGrip, GripSecondaryTag, GripBoneName, SlotName, IsSecondaryGrip, isGrabbed);
            break;
    }
}

void AxREAL_VRCharacter::TryGrabServer_Implementation(UObject *ObjectToGrab, bool IsSlotGrip, FTransform_NetQuantize GripTransform, EControllerHand Hand, FGameplayTag GripSecondaryTag, FName GripBoneName, FName SlotName, bool IsSecondaryGrip)
{
    bool isGrabbed;
    switch (Hand)
    {
        case EControllerHand::Left:
            TryToGrabObject(ObjectToGrab, GripTransform, LeftMotionController, RightMotionController, IsSlotGrip, GripSecondaryTag, GripBoneName, SlotName, IsSecondaryGrip, isGrabbed);
            break;

        case EControllerHand::Right:
            TryToGrabObject(ObjectToGrab, GripTransform, RightMotionController, LeftMotionController, IsSlotGrip, GripSecondaryTag, GripBoneName, SlotName, IsSecondaryGrip, isGrabbed);
            break;
    }
}



void AxREAL_VRCharacter::CallCorrectDropSingleEvent_Implementation(UGripMotionControllerComponent *Hand, FBPActorGripInformation Grip)
{
    // Cast object to grip interface
    IVRGripInterface *objectToDrop = Cast<IVRGripInterface>(Grip.GrippedObject);
    if (objectToDrop && IsALocalGrip(objectToDrop->GripMovementReplicationType()))
    {
        // Try Drop Single Client
    }
    else
    {
        // Try Drop Single
        FVector angularVelocity, objectsLinearVelocity;
        Hand->GetPhysicsVelocities(Grip, angularVelocity, objectsLinearVelocity);
        FVector throwingAngularVelocity, throwingLinearVelocity;
        //GetThrowingVelocity(Hand, Grip, angularVelocity, objectsLinearVelocity, throwingAngularVelocity, throwingLinearVelocity);
        //TryDropSingle_Server(Hand, Grip, throwingAngularVelocity, throwingLinearVelocity);
    }
}

void AxREAL_VRCharacter::TryDropSingle_Server_Implementation(UGripMotionControllerComponent* Hand, FVector_NetQuantize100 AngleVel, FVector_NetQuantize100 LinearVel, uint8 GripHash)
{
    FBPActorGripInformation gripInfo;
    EBPVRResultSwitch gripResult;
    Hand->GetGripByID(gripInfo, GripHash, gripResult);
    if (gripResult == EBPVRResultSwitch::OnSucceeded)
    {
        TryDropSingle_Client(Hand, gripInfo, AngleVel, LinearVel);
    }
}

void AxREAL_VRCharacter::TryDropSingle_Client_Implementation(UGripMotionControllerComponent* Hand, UPARAM(ref)FBPActorGripInformation& GripToDrop, FVector AngleVel, FVector LinearVel)
{
    bool shouldSocket;
    USceneComponent* socketParent;
    FTransform_NetQuantize relativeTransform;
    FName optSocketName;
    ShouldSocketGrip(GripToDrop, shouldSocket, socketParent, relativeTransform, optSocketName);
    
    if (shouldSocket)
    {
        Hand->DropAndSocketGrip(GripToDrop, socketParent, optSocketName, relativeTransform);
    }
    else
    {
        if (GripToDrop.GrippedObject && GripToDrop.GrippedObject->Implements<UVRGripInterface>())
        {
            Hand->DropObjectByInterface(nullptr, GripToDrop.GripID, AngleVel, LinearVel);
        }
        else
        {
            Hand->DropGrip(GripToDrop, true, AngleVel, LinearVel);
        }
    }
}

void AxREAL_VRCharacter::IfOverWidgetUse_Implementation(UGripMotionControllerComponent *CallingHand, bool Pressed, bool &WasOverWidget)
{
    EControllerHand handType;
    CallingHand->GetHandType(handType);
    switch (handType)
    {
        case EControllerHand::Left:
            WasOverWidget = false;
            if (TeleportControllerLeft->IsValidLowLevel())
            {
                TeleportControllerLeft->IfOverWidget_Use(Pressed, WasOverWidget);
            } 
            break;

        case EControllerHand::Right:
            WasOverWidget = false;
            if (TeleportControllerRight->IsValidLowLevel())
            {
                TeleportControllerRight->IfOverWidget_Use(Pressed, WasOverWidget);
            }
            break;
    }
}

void AxREAL_VRCharacter::TryRemoveSecondaryAttachment_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, FGameplayTagContainer GameplayTags, bool &DroppedSecondary, bool &HadSecondary)
{
    FBPActorGripInformation gripInfo;
    if (OtherController->GetIsSecondaryAttachment(CallingMotionController, gripInfo))
    {
        bool isMatchedOrDefault;
        ValidateGameplayTagContainer(FGameplayTag::RequestGameplayTag(TEXT("DropType.SecondaryGrip")), gripInfo.GrippedObject, DefaultSecondaryDropTag, GameplayTags, isMatchedOrDefault);
        if (isMatchedOrDefault)
        {
            if (IsALocalGrip(gripInfo.GripMovementReplicationSetting))
            {
                OtherController->RemoveSecondaryAttachmentPoint(gripInfo.GrippedObject, 0.25f);
            }
            else
            {
                RemoveSecondaryGrip_Server(OtherController, gripInfo.GrippedObject);
            }
            HadSecondary = true;
            DroppedSecondary = true;
        }
        else
        {
            HadSecondary = true;
            DroppedSecondary = false;
        }
    }
    else
    {
        HadSecondary = false;
        DroppedSecondary = false;
    }
}

void AxREAL_VRCharacter::RemoveSecondaryGrip_Server_Implementation(UGripMotionControllerComponent* Hand, UObject* GrippedActorToRemoveAttachment)
{
    Hand->RemoveSecondaryAttachmentPoint(GrippedActorToRemoveAttachment, 0.25f);
}

void AxREAL_VRCharacter::SwitchOutOfBodyCamera_Implementation(bool SwitchToOutOfBody)
{
    if (SwitchToOutOfBody)
    {
        // Go out of body
        if (!OutOfBodyCamera->IsValidLowLevel())
        {
            OutOfBodyCamera = GetWorld()->SpawnActor<ACameraActor>(ACameraActor::StaticClass());
            OutOfBodyCamera->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
            OutOfBodyCamera->SetActorTickEnabled(false);
            OutOfBodyCamera->GetCameraComponent()->SetComponentTickEnabled(false);
        }
        OutOfBodyCamera->GetCameraComponent()->SetActive(true);
        bIsOutOfBody = true;
        OutOfBodyCamera->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
        UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(OutOfBodyCamera);
        VRReplicatedCamera->bSetPositionDuringTick = true;
    }

    else
    {
        // Switch back into body
        if (bIsOutOfBody && OutOfBodyCamera->IsValidLowLevel())
        {
            OutOfBodyCamera->GetCameraComponent()->SetActive(false);
            bIsOutOfBody = false;
            OutOfBodyCamera->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true));
            UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this);
            VRReplicatedCamera->bSetPositionDuringTick = false;
        }
    }
}

void AxREAL_VRCharacter::SetTeleporterActive_Implementation(EControllerHand Hand, bool Active)
{
    switch (Hand)
    {
    case EControllerHand::Left:
        if (TeleportControllerLeft->IsValidLowLevel()) 
        {
            Active?TeleportControllerLeft->ActivateTeleporter():TeleportControllerLeft->DisableTeleporter();
            NotifyTeleportActive_Server(Hand, Active);
        }
        break;
    
    case EControllerHand::Right:
        if (TeleportControllerRight->IsValidLowLevel())
        {
            Active?TeleportControllerRight->ActivateTeleporter():TeleportControllerRight->DisableTeleporter();
            NotifyTeleportActive_Server(Hand, Active);
        }
        break;
    }
}

void AxREAL_VRCharacter::NotifyTeleportActive_Server(EControllerHand Hand, bool State)
{
    TeleportActive_Multicast(Hand, State);
}

void AxREAL_VRCharacter::TeleportActive_Multicast(EControllerHand Hand, bool State)
{
    if (!IsLocallyControlled())
    {
        switch (Hand)
        {
        case EControllerHand::Left:
            State?TeleportControllerLeft->ActivateTeleporter():TeleportControllerLeft->DisableTeleporter();
            break;
        case EControllerHand::Right:
            State?TeleportControllerRight->ActivateTeleporter():TeleportControllerRight->DisableTeleporter();
            break;
        }
    }
}

void AxREAL_VRCharacter::HandleSlidingMovement_Implementation(EVRMovementMode MovementMode, UGripMotionControllerComponent *CallingHand, bool bThumbPadInfluencesDirection)
{
    EControllerHand handType;
    CallingHand->GetHandType(handType);

    float thumbY = 0.0f;
    float thumbX = 0.0f;

    FVector direction;

    switch (handType)
    {
        case EControllerHand::Left:
            thumbY = MotionControllerThumbLeft_Y_Value; 
            thumbX = MotionControllerThumbLeft_X_Value;
            break;
        case EControllerHand::Right:
            thumbY = MotionControllerThumbRight_Y_Value;
            thumbX = MotionControllerThumbRight_X_Value;
            break;
        default:
            return;
    }

    bool  isPadCalcValid;
    float padMagnitude;
    FRotator padRotation;
    CalcPadRotationAndMagnitude(thumbY, thumbX, DPadVelocityScaler, SlidingMovementDeadZone, padRotation, padMagnitude, isPadCalcValid);

    FVector padForwardVector, padRightVector;
    GetDPadMovementFacing(MovementMode, CallingHand, nullptr, padForwardVector, padRightVector);
    if (isPadCalcValid && bThumbPadInfluencesDirection)
    {
        if (MovementMode == EVRMovementMode::DPadPress_ControllerOrient)
        {
            direction = MapThumbToWorld(padRotation, CallingHand);
        }
        else
        {
            direction = padForwardVector;
        }
    }
    else
    {
        direction = padForwardVector;
    }
    AddMovementInput(direction, padMagnitude, false);
}

void AxREAL_VRCharacter::CalcPadRotationAndMagnitude_Implementation(float YAxis, float XAxis, float OptMagnitudeScaler, float OptionalDeadzone, FRotator &Rotation, float &Magnitude, bool &WasValid)
{
    WasValid = FMath::Abs(YAxis) + FMath::Abs(XAxis) > OptionalDeadzone;
    Rotation = UKismetMathLibrary::MakeRotFromX(FVector(YAxis, XAxis, 0.0f));
    Magnitude = FMath::Clamp(FMath::Max(FMath::Abs(YAxis*OptMagnitudeScaler), FMath::Abs(XAxis*OptMagnitudeScaler)), 0.0f, 1.0f);
}

void AxREAL_VRCharacter::UpdateTeleportRotations_Implementation()
{
    // Right Controller
    if (TeleportControllerRight->IsValidLowLevel() && TeleportControllerRight->IsTeleporterActive)
    {
        if (bTeleportUsesThumbRotation)
        {
            FRotator teleportRotation;
            float magnitude;
            bool isValid;
            CalcPadRotationAndMagnitude(MotionControllerThumbRight_Y_Value, MotionControllerThumbRight_X_Value, 1.0f, TeleportThumbDeadzone, teleportRotation, magnitude, isValid);
            if (isValid)
            {
                TeleportControllerRight->TeleportRotation = teleportRotation;
            }
        }
        else
        {
            TeleportControllerRight->TeleportRotation = FRotator(0.0f, 0.0f, 0.0f);
        }
        TeleportControllerRight->TeleportBaseRotation = GetVRRotation();
    }

    // Left Controller
    if (TeleportControllerLeft->IsValidLowLevel() && TeleportControllerLeft->IsTeleporterActive)
    {
        if (bTeleportUsesThumbRotation)
        {
            FRotator teleportRotation;
            float magnitude;
            bool isValid;
            CalcPadRotationAndMagnitude(MotionControllerThumbLeft_Y_Value, MotionControllerThumbLeft_X_Value, 1.0f, TeleportThumbDeadzone, teleportRotation, magnitude, isValid);
            if (isValid)
            {
                TeleportControllerLeft->TeleportRotation = teleportRotation;
            }
        }
        else
        {
            TeleportControllerLeft->TeleportRotation = FRotator(0.0f, 0.0f, 0.0f);
        }
        TeleportControllerLeft->TeleportBaseRotation = GetVRRotation();
    }
}

void AxREAL_VRCharacter::GetCharacterRotatedPosition_Implementation(FVector OriginalLocation, FRotator DeltaRotation, FVector PivotPoint, FRotator &OutRotation, FVector &OutNewPosition, FRotator RotationToUse, FVector NewLocationOffset, FRotator NewRotation)
{
    UVRExpansionFunctionLibrary::RotateAroundPivot(DeltaRotation, OriginalLocation, GetCorrectRotation(), UKismetMathLibrary::InverseTransformLocation(GetActorTransform(), PivotPoint), OutNewPosition, OutRotation, true);
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

bool AxREAL_VRCharacter::CanObjectBeClimbed_Implementation(UPrimitiveComponent *ObjectToCheck)
{
    return false;
}

bool AxREAL_VRCharacter::HasValidGripCollision_Implementation(UPrimitiveComponent *Component)
{
    return false;
}

void AxREAL_VRCharacter::SetVehicleMode_Implementation(bool IsInVehicleMode, bool &IsVR, FRotator CorrectRotation)
{
}

bool AxREAL_VRCharacter::ShouldGripComponent_Implementation(UPrimitiveComponent *ComponentToCheck, uint8 GripPrioToCheckAgainst, bool bCheckAgainstPrior, FName BoneName, FGameplayTagContainer RelevantGameplayTags, UGripMotionControllerComponent *CallingController, UObject *&ObjectToGrip, bool &ObjectImplementsInterface, FTransform &ObjectsWorldTransform, uint8 &GripPrio)
{
    bool implementsInterface = false;
    AActor* owningActor = nullptr;
    return false;
}

void AxREAL_VRCharacter::TryToSecondaryGripObject_Implementation(UGripMotionControllerComponent *Hand, UGripMotionControllerComponent *OtherHand, UObject *ObjectToTryToGrab, FGameplayTag GripSecondaryTag, bool ObjectImplementsInterface, FTransform RelativeSecondaryTransform, FName SlotName, bool bHadSlot, bool &SecondaryGripped)
{
}

void AxREAL_VRCharacter::ClearMovementVelocities_Implementation()
{
}

void AxREAL_VRCharacter::GripOrDropObjectClean_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, bool CanCheckClimb, UPrimitiveComponent *GrabSphere, FGameplayTagContainer RelevantGameplayTags, bool &PerformedAction)
{
}

void AxREAL_VRCharacter::ValidateGameplayTagContainer_Implementation(FGameplayTag BaseTag, UObject *Object, FGameplayTag DefaultTag, FGameplayTagContainer GameplayTags, bool &MatchedOrDefault)
{
}

void AxREAL_VRCharacter::DropSecondaryAttachment_Implementation(UGripMotionControllerComponent *CallingMotionController, UGripMotionControllerComponent *OtherController, FGameplayTagContainer GameplayTags, bool &DroppedSecondary, bool &HadSecondary, bool NewLocalVar_0)
{
}

void AxREAL_VRCharacter::SelectObjectFromHitArray_Implementation(UPARAM(ref) TArray<FHitResult> &Hits, FGameplayTagContainer RelevantGameplayTags, UGripMotionControllerComponent *Hand, bool &bShouldGrip, bool &ObjectImplementsInterface, UObject *&ObjectToGrip, FTransform &WorldTransform, UPrimitiveComponent *&FirstPrimitiveHit, FName &BoneName, FVector &ImpactPoint)
{
    //uint8 bestGripPrio;
    //UObject* lOutObject;
    //FTransform lOutTransform;
    //bool lObjectImplementsInterface, lShouldGrip;
    //UPrimitiveComponent* firstHitPrimitive;
    //FName lOutBoneName;
    //FVector lImpactPoint;
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

FRotator AxREAL_VRCharacter::GetCorrectRotation_Implementation()
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

USceneComponent* AxREAL_VRCharacter::GetCorrectAimComp_Implementation(UGripMotionControllerComponent *Hand)
{
    return nullptr;
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

void AxREAL_VRCharacter::HandleCurrentMovementInput_Implementation(float MovementInput, UGripMotionControllerComponent *MovingHand, UGripMotionControllerComponent *OtherHand)
{
    bool isNotSeatedOrClimbing = VRMovementReference->GetReplicatedMovementMode() != EVRConjoinedMovementModes::C_VRMOVE_Seated && VRMovementReference->GetReplicatedMovementMode() != EVRConjoinedMovementModes::C_VRMOVE_Climbing;
    if (MovementInput > 0.0f && isNotSeatedOrClimbing && !DisableMovement)
    {
        switch (CurrentMovementMode)
        {
            case EVRMovementMode::DPadPress_ControllerOrient:
            case EVRMovementMode::DPadPress_HMDOrient:
            
                HandleSlidingMovement(CurrentMovementMode, MovingHand, bThumbPadEffectsSlidingDirection); //TODO

        }
    }
}

void AxREAL_VRCharacter::HandleTurnInput_Implementation(float InputAxis, float InputValue)
{
    if (InputAxis > 0.0f)
    {
        InputValue = bRightHandMovement ? MotionControllerThumbLeft_X_Value : MotionControllerThumbRight_X_Value;

        bool isTurningInputGreaterThanThreshold = FMath::Abs(InputValue) > TurningActivationThreshold;
        
        //Snap Turn
        if (bTurnModeIsSnap)
        {
            if (bTurningFlag)
            {
                if (!isTurningInputGreaterThanThreshold)
                {
                    bTurningFlag = false;
                }
            }
            else
            {
                if (isTurningInputGreaterThanThreshold)
                {
                    bTurningFlag = true;
                    VRMovementReference->PerformMoveAction_SnapTurn(FMath::Sign(InputValue) * SnapTurnAngle, EVRMoveActionVelocityRetention::VRMOVEACTION_Velocity_Turn, true, true, true);
                }
            }
        }

        //Smooth Turn
        else
        {
            if (isTurningInputGreaterThanThreshold)
            {
                VRMovementReference->PerformMoveAction_SnapTurn(FMath::Sign(InputValue) * SmoothTurnSpeed * GetWorld()->GetDeltaSeconds(), EVRMoveActionVelocityRetention::VRMOVEACTION_Velocity_Turn, false, false, true);
            }
        }
    }
    else
    {
        if (CurrentControllerTypeXR == EBPOpenXRControllerDeviceType::DT_ViveController || CurrentControllerTypeXR == EBPOpenXRControllerDeviceType::DT_ViveProController)
        {
            bTurningFlag = false;
        }
    }
}

void AxREAL_VRCharacter::SetMovementHands_Implementation(bool RightHandForMovement)
{
    bRightHandMovement = RightHandForMovement;
    const UEnum* EnumPtr = StaticEnum<EMovementMode>();
    if (EnumPtr)
    {
        FString enumValueName = EnumPtr->GetNameStringByValue(static_cast<int32>(CurrentMovementMode));
        //TODO: Check to make sure this works
        WriteToLog(!bRightHandMovement, enumValueName);
    }
    FString text = bTurnModeIsSnap ? "Snap Turn" : "Smooth Turn";
    WriteToLog(bRightHandMovement, text);
}

FVector AxREAL_VRCharacter::MapThumbToWorld_Implementation(FRotator PadRotation, UGripMotionControllerComponent *CallingHand)
{
    FRotator combinedRotators = UKismetMathLibrary::ComposeRotators(PadRotation, CallingHand->GetComponentRotation());
    FVector projectedVector = UKismetMathLibrary::ProjectVectorOnToPlane(combinedRotators.Vector(), GetVRUpVector());
    projectedVector.Normalize();
    return projectedVector;
}

void AxREAL_VRCharacter::OnRep_RightControllerOffset_Implementation()
{
    if (!IsLocallyControlled())
    {
        RepositionHandElements(true, RightControllerOffset);
    }
}

void AxREAL_VRCharacter::OnRep_LeftControllerOffset_Implementation()
{
    if (!IsLocallyControlled())
    {
        RepositionHandElements(false, LeftControllerOffset);
    }
}

//Input Handlers

void AxREAL_VRCharacter::ControllerMovementRight_Handler(const FInputActionValue& Value)
{

}

void AxREAL_VRCharacter::ControllerMovementLeft_Handler(const FInputActionValue& Value)
{

}

void AxREAL_VRCharacter::MotionControllerThumbLeft_X_Handler(const FInputActionValue& Value)
{
    MotionControllerThumbLeft_X_Value = Value.Get<float>();
}

void AxREAL_VRCharacter::MotionControllerThumbRight_X_Handler(const FInputActionValue& Value)
{
    MotionControllerThumbRight_X_Value = Value.Get<float>();
}

void AxREAL_VRCharacter::MotionControllerThumbLeft_Y_Handler(const FInputActionValue& Value)
{
    MotionControllerThumbLeft_Y_Value = Value.Get<float>();
}

void AxREAL_VRCharacter::MotionControllerThumbRight_Y_Handler(const FInputActionValue& Value)
{
    MotionControllerThumbRight_Y_Value = Value.Get<float>();
}
