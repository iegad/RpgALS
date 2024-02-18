#pragma once

#include "Library/ALSCharacterEnumLibrary.h"

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PropsEquipAnimNotifyState.generated.h"


UCLASS()
class ALSV4_CPP_API UPropsEquipAnimNotifyState : public UAnimNotifyState {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	EALSOverlayState OverlayState;
	
protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};