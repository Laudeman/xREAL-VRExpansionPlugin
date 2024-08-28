#pragma once

UENUM(BlueprintType)
enum class EHandAnimState : uint8 {
    
    Hand_Animated       UMETA(DisplayName = "Hand_Animated"),
    Hand_Dynamic        UMETA(DisplayName = "Hand_Dynamic"),
    Hand_Frozen         UMETA(DisplayName = "Hand_Frozen"),
    Custom              UMETA(DisplayName = "Custom"),
};