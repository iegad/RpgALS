// Copyright:	Copyright (C) 2024 iegad

#include "AnimNotifies/XReloadAnimNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XReloadAnimNotifyState)

#include "GameFramework/Character.h"

#include "Components/XGunSystemComponent.h"

void 
UXReloadAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	// TODO
}

void 
UXReloadAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	if (!MeshComp) {
		return;
	}

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOuter());
	if (!Character) {
		return;
	}

	auto GunSystem = Character->GetComponentByClass<UXGunSystemComponent>();
	if (GunSystem) {
		GunSystem->ANS_EndReload();
	}
}
