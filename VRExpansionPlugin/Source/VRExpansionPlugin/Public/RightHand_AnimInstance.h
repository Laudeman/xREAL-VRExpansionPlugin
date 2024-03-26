// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Enums/EGripState.h"
#include "RightHand_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VREXPANSIONPLUGIN_API URightHand_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Hand Animation")
		EGripState GripState;
	
};
