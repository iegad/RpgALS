// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Notifies/PropsEquipAnimNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsEquipAnimNotifyState)

#include "Character/ALSBaseCharacter.h"
#include "Components/PropsComponent.h"

void 
UPropsEquipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, 
	const FAnimNotifyEventReference& EventReference) {
	//do {
	//	if (!MeshComp) {
	//		break;
	//	}

	//	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOuter());
	//	if (!Character || !Character->GetPropsComponent()) {
	//		break;
	//	}

	//	APropsBase* Props = Character->GetPropsFromOverlayState(OverlayState);
	//	if (!Props) {
	//		break;
	//	}

	//	Character->GetPropsComponent()->StartEquip(Props);
	//} while (0);
}

void 
UPropsEquipAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference) {
	//do {
	//	if (!MeshComp) {
	//		break;
	//	}

	//	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOuter());
	//	if (!Character || !Character->GetPropsComponent()) {
	//		break;
	//	}

	//	APropsBase* Props = Character->GetPropsFromOverlayState(OverlayState);
	//	if (!Props) {
	//		break;
	//	}

	//	Character->GetPropsComponent()->EndEquip(Props);
	//} while (0);
}
