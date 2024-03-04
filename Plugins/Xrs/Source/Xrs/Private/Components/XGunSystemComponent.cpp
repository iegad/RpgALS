// Copyright:	Copyright (C) 2024 iegad

#include "Components/XGunSystemComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XGunSystemComponent)

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"

#include "Bases/XLibrary.h"
#include "Weapons/Guns/XBaseGun.h"

UXGunSystemComponent::UXGunSystemComponent() : Super() {
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void 
UXGunSystemComponent::CheckObstacle(float Limit) {
	if (CurrentGunIndex < 0) {
		return;
	}

	auto& Gun = Guns[CurrentGunIndex];
	if (!Gun) {
		return;
	}

	UWorld* World = GetWorld();
	if (!World) {
		return;
	}

	const FVector&& MuzzleLocation = Gun->GetMesh()->GetSocketLocation(Gun->GetData().MuzzleSocket);
	const FVector&& CharacterLocation = Character->GetActorLocation();
	const FVector&& Forward = Character->GetActorForwardVector();
	const FVector&& Start = CharacterLocation + Forward * Character->GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector&& End = MuzzleLocation + Forward * 150;

	FHitResult HitResult;
	World->LineTraceSingleByChannel(HitResult, Start, End, ECollisionChannel::ECC_Visibility, FCollisionQueryParams(TEXT(""), false, Character));

	if (!HitResult.bBlockingHit) {
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	if (HitActor) {
		auto M2I = FMath::Abs(FVector::Dist2D(MuzzleLocation, HitResult.ImpactPoint));
		auto C2I = FMath::Abs(FVector::Dist2D(HitResult.ImpactPoint, CharacterLocation));
		if (M2I <= Limit || M2I >= C2I) {
			OnObstacle.Broadcast(HitActor);
		}
	}
}

void
UXGunSystemComponent::Pickup(AXBaseGun* Gun) {
	if (!Gun) {
		XERROR("Gun is invalid");
		return;
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
		return;
	}

	OnPickup.Broadcast(Gun);
	OnEquip.Broadcast(Gun);
}

void 
UXGunSystemComponent::Drop(int32 index){
	XASSERT(index >= 0 && index < Guns.Num(), "index is invalid");

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
	XASSERT(index >= 0 && index < Guns.Num(), "index is invalid");

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
	XASSERT(index >= 0 && index < Guns.Num() && CurrentGunIndex < 0, "index or CurrentGunIndex is invalid");
	auto& Gun = Guns[index] = Guns[index];
	XASSERT(Gun->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, Gun->GetData().EquipSocket),
		"AttachToComponent failed");
}

void 
UXGunSystemComponent::ANS_EndEquip(int32 index) {
	XASSERT(index >= 0 && index < Guns.Num() && CurrentGunIndex < 0, "index or CurrentGunIndex is invalid");
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
	XASSERT(Gun, "Gun is nullptr");
	if (Gun->Lock()) {
		Character->PlayAnimMontage(Gun->GetData().UnequipMontage);
	}
}

void 
UXGunSystemComponent::ANS_StartUnequip() {
	XASSERT(CurrentGunIndex >= 0, "CurrentGunIndex is invalid");
	auto& Gun = Guns[CurrentGunIndex];
	XASSERT(Gun, "Gun is invalid");
	Gun->AttachToComponent(RifleAttachMesh, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void 
UXGunSystemComponent::ANS_EndUnequip() {
	XASSERT(CurrentGunIndex >= 0, "CurrentGunIndex is invalid");
	auto& Gun = Guns[CurrentGunIndex];
	XASSERT(Gun, "Gun is nullptr");
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
