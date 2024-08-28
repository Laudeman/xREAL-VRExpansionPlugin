#pragma once

UENUM(BlueprintType)
enum class ETriggerIndexes : uint8 {
    
    Thumb_3     UMETA(DisplayName = "Thumb_3"),
    Thumb_2     UMETA(DisplayName = "Thumb_2"),
    Index_3     UMETA(DisplayName = "Index_3"),
    Index_2     UMETA(DisplayName = "Index_2"),
    Middle_3    UMETA(DisplayName = "Middle_3"),
    Middle_2    UMETA(DisplayName = "Middle_2"),
    Ring_3      UMETA(DisplayName = "Ring_3"),
    Ring_2      UMETA(DisplayName = "Ring_2"),
    Pinky_3     UMETA(DisplayName = "Pinky_3"),
    Pinky_2     UMETA(DisplayName = "Pinky_2"),
};
// Enumerators as a list, so that we can iterate over them
ENUM_RANGE_BY_FIRST_AND_LAST(ETriggerIndexes, ETriggerIndexes::Thumb_3, ETriggerIndexes::Pinky_2);