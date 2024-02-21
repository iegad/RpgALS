// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Components/PropsComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsComponent)

#include "ALSLibrary.h"
#include "Character/ALSCharacter.h"
#include "Props/PropsBase.h"

UPropsComponent::UPropsComponent() : Super() {
}

void 
UPropsComponent::StartEquip(const APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSCharacter* Character = GetALSCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		const APropsBase* CurrentProps = Character->GetCurrentProps();
		if (CurrentProps) {
			UnEquip(CurrentProps);
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
UPropsComponent::EndEquip(const APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSCharacter* Character = GetALSCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Character->SetOverlayState(Props->OverlayState);
	} while (0);
}

void 
UPropsComponent::StartUnEquip(const APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSCharacter* Character = GetALSCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
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
UPropsComponent::EndUnEquip(const APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;;
		}

		AALSCharacter* Character = GetALSCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Character->SetOverlayState(EALSOverlayState::Default);

		if (DesiredProps) {
			Equip(DesiredProps);
		}
	} while (0);
}

void 
UPropsComponent::Equip(const APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Props->EquipMontage) {
			ALS_ERROR(TEXT("Props->EquipMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSCharacter* Character = GetALSCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		const APropsBase* CurrentProps = Character->GetCurrentProps();
		if (CurrentProps) {
			DesiredProps = const_cast<APropsBase*>(Props);
			UnEquip(CurrentProps);
			break;
		}

		Character->PlayAnimMontage(Props->EquipMontage);
		DesiredProps = nullptr;
	} while (0);
}

void 
UPropsComponent::UnEquip(const APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Props->UnEquipMontage) {
			ALS_ERROR(TEXT("Props->UnEquipMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSCharacter* Character = GetALSCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Character->PlayAnimMontage(Props->UnEquipMontage);
	} while (0);
}

inline AALSCharacter* 
UPropsComponent::GetALSCharacter() const {
	return Cast<AALSCharacter>(GetOwner());
}
