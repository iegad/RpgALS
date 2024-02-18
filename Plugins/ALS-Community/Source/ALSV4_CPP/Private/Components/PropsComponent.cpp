#include "Components/PropsComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsComponent)

#include "ALSLibrary.h"
#include "Character/ALSCharacter.h"
#include "Props/PropsBase.h"

UPropsComponent::UPropsComponent() : Super() {
}

void 
UPropsComponent::StartEquip(AALSCharacter* Character, const APropsBase* Props) const {
	do {
		if (!Character || !Props) {
			ALS_ERROR(TEXT("Character or Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		const APropsBase* CurrentProps = Character->GetCurrentProps();
		if (CurrentProps) {
			UnEquip(Character, CurrentProps);
			break;
		}

		UChildActorComponent* ChildActor = nullptr;
		switch (Props->OverlayState) {
		case EALSOverlayState::Rifle:
			ChildActor = Character->ChildActorRifle;
			break;

		case EALSOverlayState::PistolOneHanded:
			ChildActor = Character->ChildActorPistol;
			break;

		default: 
			break;
		}

		if (!ChildActor) {
			ALS_ERROR(TEXT("ChildActor is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		ChildActor->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, Props->EquipSocketName);
	} while (0);
}

void 
UPropsComponent::EndEquip(AALSCharacter* Character, const APropsBase* Props) const {
	if (Character && Props) {
		Character->SetOverlayState(Props->OverlayState);
		ALS_LOG(TEXT("SetOverlayState: %d"), Props->OverlayState);
	}
}

void 
UPropsComponent::StartUnEquip(AALSCharacter* Character, const APropsBase* Props) const {
	do {
		if (!Character || !Props) {
			ALS_ERROR(TEXT("Character or Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UChildActorComponent* ChildActor = nullptr;
		USceneComponent* Scene = nullptr;

		switch (Character->GetOverlayState()) {
		case EALSOverlayState::Rifle:
			ChildActor = Character->ChildActorRifle;
			Scene = Character->SceneRifle;
			break;

		case EALSOverlayState::PistolOneHanded:
			ChildActor = Character->ChildActorPistol;
			Scene = Character->ScenePistol;
			break;

		default:
			break;
		}

		if (!ChildActor || !Scene) {
			ALS_ERROR(TEXT("ChildActor or Scene is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		ChildActor->AttachToComponent(Scene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	} while (0);
}

void 
UPropsComponent::EndUnEquip(AALSCharacter* Character, const APropsBase* Props) const {
	do {
		if (!Character || !Props) {
			ALS_ERROR(TEXT("Character or Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;;
		}

		Character->SetOverlayState(EALSOverlayState::Default);

		if (DesiredProps) {
			Equip(Character, DesiredProps);
		}
	} while (0);
}

void 
UPropsComponent::Equip(AALSCharacter* Character, const APropsBase* Props) const {
	do {
		if (!Character || !Props) {
			ALS_ERROR(TEXT("Character or Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Props->EquipMontage) {
			ALS_ERROR(TEXT("Props->EquipMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		const APropsBase* CurrentProps = Character->GetCurrentProps();
		if (CurrentProps) {
			DesiredProps = const_cast<APropsBase*>(Props);
			UnEquip(Character, CurrentProps);
			break;
		}

		Character->PlayAnimMontage(Props->EquipMontage);
		DesiredProps = nullptr;
	} while (0);
}

void 
UPropsComponent::UnEquip(AALSCharacter* Character, const APropsBase* Props) const {
	do {
		if (!Character || !Props) {
			ALS_ERROR(TEXT("Character or Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Props->UnEquipMontage) {
			ALS_ERROR(TEXT("Props->UnEquipMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Character->PlayAnimMontage(Props->UnEquipMontage);
	} while (0);
}
