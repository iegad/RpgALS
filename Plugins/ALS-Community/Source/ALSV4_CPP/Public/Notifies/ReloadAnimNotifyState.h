#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "ReloadAnimNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class ALSV4_CPP_API UReloadAnimNotifyState : public UAnimNotifyState {
	GENERATED_BODY()
	
protected:
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
