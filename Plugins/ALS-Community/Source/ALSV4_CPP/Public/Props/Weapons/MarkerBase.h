// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "ALSActor.h"
#include "MarkerBase.generated.h"

class UDecalComponent;

UCLASS()
class ALSV4_CPP_API AMarkerBase : public AALSActor {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UDecalComponent> Decal;

public:
	AMarkerBase();
};
