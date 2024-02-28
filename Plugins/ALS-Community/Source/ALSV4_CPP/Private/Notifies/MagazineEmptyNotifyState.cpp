#include "Notifies/MagazineEmptyNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(MagazineEmptyNotifyState)

#include "Character/ALSBaseCharacter.h"
#include "Components/PropsComponent.h"
#include "Props/Weapons/Guns/GunBase.h"

void 
UMagazineEmptyNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,
	const FAnimNotifyEventReference& EventReference) {
	do {
		if (!MeshComp) {
			break;
		}

		AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOuter());
		if (!Character || !Character->GetPropsComponent()) {
			break;
		}

		AGunBase* GunBase = Cast<AGunBase>(Character->GetCurrentProps());
		if (!GunBase) {
			break;
		}

		if (Character->GetPropsComponent()) {
			Character->GetPropsComponent()->EjectMagazine(GunBase);
		}
	} while (0);
}

void 
UMagazineEmptyNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference) {
	do {
		if (!MeshComp) {
			break;
		}

		AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOuter());
		if (!Character || !Character->GetPropsComponent()) {
			break;
		}

		AGunBase* GunBase = Cast<AGunBase>(Character->GetCurrentProps());
		if (!GunBase) {
			break;
		}

		if (Character->GetPropsComponent()) {
			Character->GetPropsComponent()->DropMagazine(GunBase);
		}
	} while (0);
}
