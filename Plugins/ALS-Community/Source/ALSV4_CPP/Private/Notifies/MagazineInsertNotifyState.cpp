#include "Notifies/MagazineInsertNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(MagazineInsertNotifyState)

#include "Character/ALSBaseCharacter.h"
#include "Components/PropsComponent.h"
#include "Props/Weapons/Guns/GunBase.h"

void
UMagazineInsertNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, 
	const FAnimNotifyEventReference& EventReference) {
	//do {
	//	if (!MeshComp) {
	//		break;
	//	}

	//	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOuter());
	//	if (!Character || !Character->GetPropsComponent()) {
	//		break;
	//	}

	//	AGunBase* GunBase = Cast<AGunBase>(Character->GetCurrentProps());
	//	if (!GunBase) {
	//		break;
	//	}

	//	if (Character->GetPropsComponent()) {
	//		Character->GetPropsComponent()->StartInsertMagazine(GunBase);
	//	}
	//} while (0);
}

void 
UMagazineInsertNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
	const FAnimNotifyEventReference& EventReference) {
	//do {
	//	if (!MeshComp) {
	//		break;
	//	}

	//	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOuter());
	//	if (!Character || !Character->GetPropsComponent()) {
	//		break;
	//	}

	//	AGunBase* GunBase = Cast<AGunBase>(Character->GetCurrentProps());
	//	if (!GunBase) {
	//		break;
	//	}

	//	if (Character->GetPropsComponent()) {
	//		Character->GetPropsComponent()->EndInsertMagazine(GunBase);
	//	}
	//} while (0);
}
