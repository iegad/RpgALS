// Copyright:	Copyright (C) 2024 iegad

#include "AnimNotifies/XUnequipAnimNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XUnequipAnimNotifyState)

#include "GameFramework/Character.h"

#include "Components/XGunSystemComponent.h"

void 
UXUnequipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	if (!MeshComp) {
		return;
	}

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOuter());
	if (!Character) {
		return;
	}

	auto GunSystem = Character->GetComponentByClass<UXGunSystemComponent>();
	if (GunSystem) {
		GunSystem->ANS_StartUnequip();
	}
}

void 
UXUnequipAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	if (!MeshComp) {
		return;
	}

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOuter());
	if (!Character) {
		return;
	}

	auto GunSystem = Character->GetComponentByClass<UXGunSystemComponent>();
	if (GunSystem) {
		GunSystem->ANS_EndUnequip();
	}
}
