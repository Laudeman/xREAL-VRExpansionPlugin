// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsTossManager.h"
#include "Kismet/GameplayStatics.h"

void UPhysicsTossManager::ServersideToss_Implementation(UPrimitiveComponent* TargetObject, UGripMotionControllerComponent* TargetMotionController)
{
    if (!ObjectBeingThrown->IsValidLowLevel())
    {
        if (TargetObject->IsValidLowLevel())
        {
            IVRGripInterface* objectOwnerGripInterface = Cast<IVRGripInterface>(TargetObject->GetOwner());
            if (objectOwnerGripInterface)
            {
                bool isHeld = nullptr;
                TArray<FBPGripPair> holdingControllers;
                objectOwnerGripInterface->IsHeld(holdingControllers, isHeld);
                if (!isHeld)
                {
                    OwningMotionController = TargetMotionController;
                    ObjectBeingThrown = TargetObject;
                    TargetObject->OnComponentHit.AddDynamic(this, &UPhysicsTossManager::CancelThrow);
                    ObjectBeingThrown->SetSimulatePhysics(true);
                    RunToss(isHeld);
                    ToggleTick();
                }
            }
        }
    }
    ObjectBeingThrown = TargetObject;
    OwningMotionController = TargetMotionController;
    ToggleTick();
}

void UPhysicsTossManager::RunToss_Implementation(bool IsHeld)
{
    FVector throwVelocity = OwningMotionController->GetPivotLocation() - ObjectBeingThrown->GetComponentLocation();
    float currentDistanceSQ = throwVelocity.SizeSquared();
    FVector normalVector = throwVelocity.GetSafeNormal(0.0001f);

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
            CancelToss();
        }
    }
    else
    {
        CancelToss();
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
