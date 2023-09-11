#pragma once

UENUM(BlueprintType)
enum class EGripState : uint8 {
    
    Open        UMETA(DisplayName = "Open"),
    CanGrab     UMETA(DisplayName = "CanGrab"),
    Grab        UMETA(DisplayName = "Grab"),
};