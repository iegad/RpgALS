// Copyright:	Copyright (C) 2024 iegad

#include "Components/XGunSystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XGunSystemComponent)

#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "Bases/XLibrary.h"
#include "Weapons/Guns/XBaseGun.h"

UXGunSystemComponent::UXGunSystemComponent() : Super() {
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void
UXGunSystemComponent::Pickup(AXBaseGun* Gun) {
	do {
		if (!Gun) {
			XERROR("Gun is invalid");
			break;
		}

		if (CurrentGunIndex >= 0) {
			Drop(CurrentGunIndex);
		}

		int32 index = (int32)EXGunType::EGT_Rifle - 1;
		if (Guns[index]) {
			Drop(index);
		}

		CurrentGunIndex = index;
		Guns[CurrentGunIndex] = Gun;

		Gun->DisableCollision();
		if (!Gun->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Gun->GetData().EquipSocket)) {
			XERROR("Pickup failed");
			break;
		}

		OnPickup.Broadcast(Gun);
	} while (0);
}

void 
UXGunSystemComponent::Drop(int32 index){
	check(index >= 0 && index < Guns.Num());

	auto& CurrentGun = Guns[index];
	CurrentGun->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	CurrentGun->SetActorTransform(Character->GetTransform());
	CurrentGun->EnableCollision();
	OnDrop.Broadcast(CurrentGun);

	if (index == CurrentGunIndex) {
		CurrentGunIndex = -1;
		CurrentGun = nullptr;
	}
}

void 
UXGunSystemComponent::Equip(int32 index) {
	 check(index >= 0 && index < Guns.Num());

	if (CurrentGunIndex >= 0) {
		DesiredEquipGunIndex = index;
		Unequip();
		return;
	}

	auto& Gun = Guns[index];

	if (Gun && Gun->Lock()) {
		Character->PlayAnimMontage(Gun->GetData().EquipMontage);
		if (DesiredEquipGunIndex >= 0) {
			DesiredEquipGunIndex = -1;
		}
	}
}

void 
UXGunSystemComponent::ANS_StartEquip(int32 index) {
	check(index >= 0 && index < Guns.Num() && CurrentGunIndex < 0);
	auto& Gun = Guns[index] = Guns[index];
	check(Gun->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Gun->GetData().EquipSocket));
}

void 
UXGunSystemComponent::ANS_EndEquip(int32 index) {
	check(index >= 0 && index < Guns.Num() && CurrentGunIndex < 0);
	CurrentGunIndex = index;
	auto& Gun = Guns[index];
	Gun->UnLock();
	OnEquip.Broadcast(Gun);
}

void 
UXGunSystemComponent::Unequip() {
	if (CurrentGunIndex < 0) {
		return;
	}

	auto& Gun = Guns[CurrentGunIndex];
	check(Gun);
	if (Gun->Lock()) {
		Character->PlayAnimMontage(Gun->GetData().UnequipMontage);
	}
}

void 
UXGunSystemComponent::ANS_StartUnequip() {
	check(CurrentGunIndex >= 0);
	auto& Gun = Guns[CurrentGunIndex];
	check(Gun);
	Gun->AttachToComponent(RifleAttachMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void 
UXGunSystemComponent::ANS_EndUnequip() {
	check(CurrentGunIndex >= 0);
	auto& Gun = Guns[CurrentGunIndex];
	check(Gun);
	Gun->UnLock();

	OnUnequip.Broadcast(Gun);
	CurrentGunIndex = -1;
	
	if (DesiredEquipGunIndex >= 0) {
		Equip(DesiredEquipGunIndex);
	}
}

void 
UXGunSystemComponent::Fire() {
}

void 
UXGunSystemComponent::Reload() {
	if (CurrentGunIndex < 0) {
		return;
	}

	auto& Gun = Guns[CurrentGunIndex];
	if (!Gun) {
		return;
	}

	auto& Data = Gun->GetData();
	if (Data.CurrentAmmo >= Data.ClipAmmo || Data.ReserveAmmo == 0) {
		return;
	}

	if (!Gun->Lock()) {
		return;
	}

	Character->PlayAnimMontage(Data.ReloadMontage);
}

void 
UXGunSystemComponent::ANS_StartRealod() {
	// TODO
}

void 
UXGunSystemComponent::ANS_EndReload() {
	if (CurrentGunIndex < 0) {
		return;
	}

	auto& Gun = Guns[CurrentGunIndex];
	if (!Gun) {
		return;
	}

	auto& Data = Gun->GetData();
	int32 ReloadValue = Data.ClipAmmo - Data.CurrentAmmo;
	if (ReloadValue > Data.ReserveAmmo) {
		ReloadValue = Data.ReserveAmmo;
	}

	Data.ReserveAmmo -= ReloadValue;
	Data.CurrentAmmo += ReloadValue;

	Gun->UnLock();
	OnReload.Broadcast(Gun);
}

void
UXGunSystemComponent::BeginPlay() {
	Super::BeginPlay();	

	Character = Cast<ACharacter>(GetOuter());
	if (!Character) {
		XFATAL("XGunComponent must be mounted on the Character");
	}

	if (NameRifleAttachMesh.IsNone()) {
		return;
	}

	RifleAttachMesh = Cast<UStaticMeshComponent>(Character->GetDefaultSubobjectByName(NameRifleAttachMesh));
	if (!RifleAttachMesh) {
		XERROR("Character don't have StaticMeshComponent named %s", *NameRifleAttachMesh.ToString());
	}
}
