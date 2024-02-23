// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Notifies/PropsUnEquipAnimNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsUnEquipAnimNotifyState)

#include "Character/ALSCharacter.h"
#include "Components/PropsComponent.h"

void 
UPropsUnEquipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, 
	const FAnimNotifyEventReference& EventReference) {
	do {
		if (!MeshComp) {
			break;
		}

		AALSCharacter* Character = Cast<AALSCharacter>(MeshComp->GetOuter());
		if (!Character || !Character->PropsComponent) {
			break;
		}

		APropsBase* Props = Character->GetPropsFromOverlayState(OverlayState);
		if (!Props) {
			break;
		}

		Character->PropsComponent->StartUnEquip(Props);
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
		if (!Character || !Character->PropsComponent) {
			break;
		}

		APropsBase* Props = Character->GetPropsFromOverlayState(OverlayState);
		if (!Props) {
			break;
		}

		Character->PropsComponent->EndUnEquip(Props);
	} while (0);
}
