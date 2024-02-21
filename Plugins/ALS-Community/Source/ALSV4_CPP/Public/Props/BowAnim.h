// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BowAnim.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UBowAnim : public UAnimInstance {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Draw = 0.f;
};
