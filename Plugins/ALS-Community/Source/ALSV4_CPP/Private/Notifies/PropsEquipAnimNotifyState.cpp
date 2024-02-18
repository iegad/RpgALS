#include "Notifies/PropsEquipAnimNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsEquipAnimNotifyState)

#include "Character/ALSCharacter.h"
#include "Components/WeaponComponent.h"

void 
UPropsEquipAnimNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, 
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

		Character->WeaponComponent->StartEquip(Character, Props);
	} while (0);
}

void 
UPropsEquipAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
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

		Character->WeaponComponent->EndEquip(Character, Props);
	} while (0);
}
