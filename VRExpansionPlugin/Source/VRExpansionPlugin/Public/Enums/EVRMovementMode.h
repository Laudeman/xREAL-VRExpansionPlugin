#pragma once

UENUM(BlueprintType)
enum class EVRMovementMode : uint8
{
    Teleport                    UMETA(DisplayName="Teleport"),
    Navigate                    UMETA(DisplayName="Navigate"),
    OutOfBodyNavigation         UMETA(DisplayName="OutOfBodyNavigation"),
    DPadPress_ControllerOrient  UMETA(DisplayName="DPadPress_ControllerOrient"),
    DPadPress_HMDOrient         UMETA(DisplayName="DPadPress_HMDOrient"),
    RunInPlace                  UMETA(DisplayName="RunInPlace"),
    RunInPlaceHeadForward       UMETA(DisplayName="RunInPlaceHeadForward"),
    ArmSwing                    UMETA(DisplayName="ArmSwing"),
    ClimbingMode            UMETA(DisplayName="ClimbingMode"),

};