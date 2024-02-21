// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Notifies/PropsUnEquipAnimNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsUnEquipAnimNotifyState)

#include "Character/ALSCharacter.h"
#include "Components/WeaponComponent.h"

void 
UPropsUnEquipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, 
	const FAnimNotifyEventReference& EventReference) {
	do {
		if (!MeshComp) {
			break;
		}

		AALSCharacter* Character = Cast<AALSCharacter>(MeshComp->GetOuter());
		if (!Character || !Character->WeaponComponent) {
			break;
		}

		APropsBase* Props = Character->GetPropsFromOverlayState(OverlayState);
		if (!Props) {
			break;
		}

		Character->WeaponComponent->StartUnEquip(Props);
	} while (0);
}

void 
UPropsUnEquipAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference) {
	do {
		if (!MeshComp) {
			break;
		}

		AALSCharacter* Character = Cast<AALSCharacter>(MeshComp->GetOuter());
		if (!Character || !Character->WeaponComponent) {
			break;
		}

		APropsBase* Props = Character->GetPropsFromOverlayState(OverlayState);
		if (!Props) {
			break;
		}

		Character->WeaponComponent->EndUnEquip(Props);
	} while (0);
}
