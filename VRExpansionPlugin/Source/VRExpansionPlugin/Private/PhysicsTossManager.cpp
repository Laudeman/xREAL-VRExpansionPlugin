// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsTossManager.h"
#include "Kismet/GameplayStatics.h"

UPhysicsTossManager::UPhysicsTossManager(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryComponentTick.bStartWithTickEnabled = false;
    PrimaryComponentTick.bCanEverTick = true;
    DistanceToStartSlowing = 100.0f;
    MinimumSpeed = 200.0f;
    CancelDistance = 20.0f;
}

void UPhysicsTossManager::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    RunThrowing();
}

void UPhysicsTossManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    Super::EndPlay(EndPlayReason);
    CancelToss();
}

void UPhysicsTossManager::ServersideToss_Implementation(UPrimitiveComponent* TargetObject, UGripMotionControllerComponent* TargetMotionController)
{
    if (!ObjectBeingThrown->IsValidLowLevel())
    {
        if (TargetObject->IsValidLowLevel())
        {
            //IVRGripInterface* objectOwnerGripInterface = Cast<IVRGripInterface>(TargetObject->GetOwner());
            if (TargetObject->GetOwner()->Implements<UVRGripInterface>())
            {
                bool isHeld = false;
                TArray<FBPGripPair> holdingControllers;
                IVRGripInterface::Execute_IsHeld(TargetObject->GetOwner(), holdingControllers, isHeld);
                if (!isHeld)
                {
                    OwningMotionController = TargetMotionController;
                    ObjectBeingThrown = TargetObject;
                    ObjectBeingThrown->OnComponentHit.AddDynamic(this, &UPhysicsTossManager::CancelThrow);
                    ObjectBeingThrown->SetSimulatePhysics(true);
                    RunToss(isHeld);
                    ToggleTick();
                }
            }
        }
    }
}

void UPhysicsTossManager::RunToss_Implementation(bool IsHeld)
{
    FVector throwVelocity = OwningMotionController->GetPivotLocation() - ObjectBeingThrown->GetComponentLocation();
    float currentDistanceSQ = throwVelocity.SizeSquared();
    FVector normalVector = throwVelocity;
    normalVector.Normalize(0.0001f);

    if (currentDistanceSQ > FMath::Square(CancelDistance) && !IsHeld)
    {
        FVector outLaunchVelocity;
        if (UGameplayStatics::SuggestProjectileVelocity_CustomArc(GetWorld(), outLaunchVelocity, ObjectBeingThrown->GetComponentLocation(), OwningMotionController->GetPivotLocation(), 0.0f, 0.8f))
        {
            
            float vectorLengthSquared = FVector::VectorPlaneProject(outLaunchVelocity, normalVector).SizeSquared();

            float clampedSpeed = FMath::Clamp(FMath::Square(MinimumSpeed) - vectorLengthSquared, 0.0f, MinimumSpeed);

            float rangeSlowDownFloat = FMath::Clamp(1 - FMath::Square(DistanceToStartSlowing) / currentDistanceSQ, 0.8f, 1.0f);

            FVector newVelocity = (normalVector * clampedSpeed) + outLaunchVelocity * rangeSlowDownFloat;

            ObjectBeingThrown->SetPhysicsLinearVelocity(newVelocity, false);
        }
        else
        {
            CancelThrowing(ObjectBeingThrown);
        }
    }
    else
    {
        CancelThrowing(ObjectBeingThrown);
    }
}

void UPhysicsTossManager::RunThrowing_Implementation()
{
    if (ObjectBeingThrown->IsValidLowLevel())
    {
        bool isHeld = false;
        TArray<FBPGripPair> holdingControllers;
        // Use this if you want to check the owner instead but the blueprint implementation didn't do this.
        //if (ObjectBeingThrown->GetOwner()->Implements<UVRGripInterface>())
        if (ObjectBeingThrown->Implements<UVRGripInterface>())
        {
            //IVRGripInterface::Execute_IsHeld(ObjectBeingThrown->GetOwner(), holdingControllers, isHeld);
            IVRGripInterface::Execute_IsHeld(ObjectBeingThrown, holdingControllers, isHeld);
        }
        RunToss(isHeld);
    }
    else
    {
        CancelThrowing(ObjectBeingThrown);
    }
}

void UPhysicsTossManager::CancelToss_Implementation()
{
    CancelThrowing(ObjectBeingThrown);
}

void UPhysicsTossManager::CancelThrowing_Implementation(UPrimitiveComponent* ObjectToCancel)
{
    if (ObjectToCancel->IsValidLowLevel())
    {
        ObjectToCancel->OnComponentHit.RemoveDynamic(this, &UPhysicsTossManager::CancelThrow);
    }
    ObjectBeingThrown = nullptr;
    ToggleTick();
}

void UPhysicsTossManager::CancelThrow_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    CancelThrowing(HitComponent);
}



void UPhysicsTossManager::IsThrowing_Implementation(bool &Throwing)
{
    Throwing = ObjectBeingThrown->IsValidLowLevel();
}

void UPhysicsTossManager::ToggleTick_Implementation()
{
    SetComponentTickEnabled(ObjectBeingThrown->IsValidLowLevel());
}
