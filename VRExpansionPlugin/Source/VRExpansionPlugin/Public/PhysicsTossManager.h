// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GripMotionControllerComponent.h"
#include "PhysicsTossManager.generated.h"


//UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
UCLASS(Blueprintable, BlueprintType)
class VREXPANSIONPLUGIN_API UPhysicsTossManager : public UActorComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent)
	void ServersideToss(UPrimitiveComponent* TargetObject, UGripMotionControllerComponent* TargetMotionController);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void RunToss(bool IsHeld);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void CancelToss();

	UFUNCTION(BlueprintNativeEvent)
	void CancelThrowing(UPrimitiveComponent* ObjectToCancel);

	UFUNCTION(BlueprintNativeEvent)
	void CancelThrow(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent, BlueprintPure)
	void IsThrowing(bool& Throwing);

	/** Please add a function description */
	UFUNCTION(BlueprintNativeEvent)
	void ToggleTick();
public:
	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(MultiLine="true", ExposeOnSpawn="true"))
	TObjectPtr<UGripMotionControllerComponent> OwningMotionController;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(MultiLine="true"))
	double DistanceToStartSlowing;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(MultiLine="true"))
	double MinimumSpeed;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category="Default", meta=(MultiLine="true"))
	double CancelDistance;

	/** Please add a variable description */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category="Default", meta=(MultiLine="true"))
	TObjectPtr<UPrimitiveComponent> ObjectBeingThrown;

		
};
