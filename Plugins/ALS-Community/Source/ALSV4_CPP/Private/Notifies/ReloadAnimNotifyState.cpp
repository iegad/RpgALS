#include "Notifies/ReloadAnimNotifyState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ReloadAnimNotifyState)

#include "Character/ALSBaseCharacter.h"
#include "Components/PropsComponent.h"
#include "Props/Weapons/Guns/GunBase.h"

void 
UReloadAnimNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, 
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
	//		Character->GetPropsComponent()->EndReload(GunBase);
	//	}
	//} while (0);
}
