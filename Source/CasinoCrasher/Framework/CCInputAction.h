// Copyright 2024 Thomas Ballantyne. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "CCInputAction.generated.h"

/**
 * Input Action subclass which has a mechanism to provide unique int32 values 
 * for use with the Ability System
 */
UCLASS()
class CASINOCRASHER_API UCCInputAction : public UInputAction
{
    GENERATED_BODY()

protected:
    //FGuids are globally unique.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="GUID")
    FGuid InputActionGuid;

public:
    /**
     * @brief Creates a unique int32 value from the GUID of this Input Action
     * @return InputID which can be used with the Ability System
     */
    int32 GetInputID() const
    {
        //FGuids are globally unique. They are made out of four int32 values
        //i.e. the quadruple of these int32 values is unique
        //and so adding them up together, 
        //returns a globally unique int32 value
        
        return static_cast<int32>(InputActionGuid.A)
        + static_cast<int32>(InputActionGuid.B)
        + static_cast<int32>(InputActionGuid.C)
        + static_cast<int32>(InputActionGuid.D);
    }
};