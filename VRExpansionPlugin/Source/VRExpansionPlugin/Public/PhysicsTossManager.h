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

	UPhysicsTossManager(const FObjectInitializer& ObjectInitializer);

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ServersideToss(UPrimitiveComponent* TargetObject, UGripMotionControllerComponent* TargetMotionController);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RunToss(bool IsHeld);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RunThrowing();

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CancelToss();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CancelThrowing(UPrimitiveComponent* ObjectToCancel);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CancelThrow(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, BlueprintPure)
	void IsThrowing(bool& Throwing);

	/** Please add a function description */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
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
