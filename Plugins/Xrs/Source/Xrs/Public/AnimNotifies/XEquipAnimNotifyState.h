// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "XEquipAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class XRS_API UXEquipAnimNotifyState : public UAnimNotifyState {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category = "Xrs")
	int32 GunIndex = -1;

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
