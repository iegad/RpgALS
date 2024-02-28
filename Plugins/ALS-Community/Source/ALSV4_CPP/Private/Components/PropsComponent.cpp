// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Components/PropsComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsComponent)

#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ALSLibrary.h"
#include "Character/ALSBaseCharacter.h"
#include "Core/ALSGameInstance.h"
#include "Props/Weapons/Guns/GunBase.h"
#include "Props/Weapons/Guns/Magazine.h"
#include "Props/Weapons/MarkerBase.h"
#include "Props/Weapons/TracerBase.h"
#include "Core/ALSGameMode.h"
#include "UI/ALSPlayerHUD.h"

static const FName MagazineSocketName{ TEXT("Slot_Magazine") };

UPropsComponent::UPropsComponent() : Super() {
}

void 
UPropsComponent::StartEquip(APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		APropsBase* CurrentProps = Character->GetCurrentProps();
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
UPropsComponent::EndEquip(APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Character->SetOverlayState(Props->OverlayState);

		UALSPlayerHUD* HUD = Character->GetHUD();
		if (HUD) {
			AGunBase* Gun = Cast<AGunBase>(Props);
			if (Gun) {
				HUD->SetGun(Gun);
			}
		}

		Props->UnLock();
	} while (0);
}

void 
UPropsComponent::EndReload(AGunBase* Gun) const {
	if (!Gun) {
		return;
	}

	AALSBaseCharacter* Character = GetALSBaseCharacter();
	if (!Character) {
		return;
	}

	FALSAmmoInfo& AmmoInfo = Gun->AmmoInfo;
	int32 ReloadValue = AmmoInfo.ClipAmmo - AmmoInfo.CurrentAmmo;
	if (ReloadValue > AmmoInfo.MaxAmmo) {
		ReloadValue = AmmoInfo.MaxAmmo;
	}

	AmmoInfo.MaxAmmo -= ReloadValue;
	AmmoInfo.CurrentAmmo += ReloadValue;

	UALSPlayerHUD* HUD = Character->GetHUD();
	if (HUD) {
		HUD->SetCurrentAmmo(AmmoInfo.CurrentAmmo);
		HUD->SetMaxAmmo(AmmoInfo.MaxAmmo);
	}

	Gun->UnLock();
}

void 
UPropsComponent::StartUnEquip(APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
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
UPropsComponent::EndUnEquip(APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UALSPlayerHUD* HUD = Character->GetHUD();
		if (HUD) {
			HUD->ResetGun();
		}

		Props->UnLock();
		Character->SetOverlayState(EALSOverlayState::Default);

		if (DesiredProps) {
			Equip(DesiredProps);
		}
	} while (0);
}

void 
UPropsComponent::EjectMagazine(AGunBase* Gun) const {
	do {
		if (!Gun) {
			ALS_ERROR(TEXT("Gun is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Gun->MagazineEmpty) {
			ALS_ERROR(TEXT("Gun->MagazineEmpty is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UWorld* World = GetWorld();
		if (!World) {
			ALS_ERROR(TEXT("World is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UALSGameInstance* GameInstance = Cast<UALSGameInstance>(World->GetGameInstance());
		if (!GameInstance) {
			ALS_ERROR(TEXT("GameInstance is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("Character is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		// 隐藏枪的弹匣
		Gun->Mesh->HideBoneByName(Gun->MagazineSock, EPhysBodyOp::PBO_None);
		MagazineEmpty = GameInstance->ALSActorPool->Get<AMagazine>(World, Gun->MagazineEmpty, 5.f, Character->GetMesh(), MagazineSocketName);
		check(MagazineEmpty);
	} while (0);
}

void 
UPropsComponent::DropMagazine(AGunBase* Gun) const {
	do {
		if (!Gun) {
			ALS_ERROR(TEXT("Gun is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!MagazineEmpty) {
			ALS_ERROR(TEXT("MagazineEmpty is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UWorld* World = GetWorld();
		if (!World) {
			ALS_ERROR(TEXT("World is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		MagazineEmpty->Mesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		MagazineEmpty->Mesh->SetSimulatePhysics(true);
		MagazineEmpty = nullptr;
	} while (0);
}

void 
UPropsComponent::StartInsertMagazine(AGunBase* Gun) const {
	do {
		if (!Gun) {
			ALS_ERROR(TEXT("Gun is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Gun->MagazineFull) {
			ALS_ERROR(TEXT("Gun->MagazineFull is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UWorld* World = GetWorld();
		if (!World) {
			ALS_ERROR(TEXT("World is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UALSGameInstance* GameInstance = Cast<UALSGameInstance>(World->GetGameInstance());
		if (!GameInstance) {
			ALS_ERROR(TEXT("GameInstance is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("Character is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		MagazineFull = GameInstance->ALSActorPool->Get<AMagazine>(World, Gun->MagazineFull, 0.f, Character->GetMesh(), MagazineSocketName);
		check(MagazineFull);
	} while (0);
}

void 
UPropsComponent::EndInsertMagazine(AGunBase* Gun) const {
	do {
		if (!Gun) {
			ALS_ERROR(TEXT("Gun is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!MagazineFull) {
			ALS_ERROR(TEXT("MagazineFull is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UWorld* World = GetWorld();
		if (!World) {
			ALS_ERROR(TEXT("World is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UALSGameInstance* GameInstance = Cast<UALSGameInstance>(World->GetGameInstance());
		if (!GameInstance) {
			ALS_ERROR(TEXT("GameInstance is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		GameInstance->ALSActorPool->Put(MagazineFull);
		MagazineFull = nullptr;

		Gun->Mesh->UnHideBoneByName(Gun->MagazineSock);
	} while (0);
}

void 
UPropsComponent::Equip(APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Props->EquipMontage) {
			ALS_ERROR(TEXT("Props->EquipMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (Props->GetLock()) {
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		APropsBase* CurrentProps = Character->GetCurrentProps();
		if (CurrentProps) {
			DesiredProps = Props;
			UnEquip(CurrentProps);
			break;
		}

		Props->Lock();
		Character->PlayAnimMontage(Props->EquipMontage);
		DesiredProps = nullptr;
	} while (0);
}

void 
UPropsComponent::UnEquip(APropsBase* Props) const {
	do {
		if (!Props) {
			ALS_ERROR(TEXT("Props is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Props->UnEquipMontage) {
			ALS_ERROR(TEXT("Props->UnEquipMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (Props->GetLock()) {
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Props->Lock();
		Character->PlayAnimMontage(Props->UnEquipMontage);
	} while (0);
}

void 
UPropsComponent::Attack(AWeaponBase* Weapon, int DebugTrace) const {
	if (!Weapon) {
		return;
	}

	AGunBase* Gun = Cast<AGunBase>(Weapon);
	if (Gun) {
		AttackInternal(Gun, DebugTrace);
		return;
	}
	
	AttackInternal(Weapon, DebugTrace);
}

void 
UPropsComponent::Reload(AGunBase* Gun) const {
	do {
		if (!Gun) {
			ALS_ERROR(TEXT("Gun is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("Character is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Gun->ReloadMontage) {
			ALS_ERROR(TEXT("ReloadMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		FALSAmmoInfo& AmmoInfo = Gun->AmmoInfo;
		if (AmmoInfo.CurrentAmmo >= AmmoInfo.ClipAmmo || AmmoInfo.MaxAmmo == 0) {
			break;
		}

		if (Gun->GetLock()) {
			break;
		}

		Gun->Lock();
		Character->PlayAnimMontage(Gun->ReloadMontage);
	} while (0);
}

inline AALSBaseCharacter*
UPropsComponent::GetALSBaseCharacter() const {
	return Cast<AALSBaseCharacter>(GetOwner());
}

void 
UPropsComponent::AttackInternal(AWeaponBase* Weapon, int DebugTrace) const {
	// TODO: ...
}

void 
UPropsComponent::AttackInternal(AGunBase* Gun, int DebugTrace) const {
	do {
		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("Character is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (Gun->WeaponAttackOptions.Range <= 0.f) {
			ALS_ERROR(TEXT("%s Attack Range is 0: %s:%d"), *this->GetName(), __FILEW__, __LINE__);
			break;
		}

		if (Gun->WeaponAttackOptions.AttackCD > 0) {
			break;
		}

		if (Gun->GetLock()) {
			break;
		}

		UWorld* World = GetWorld();
		if (!World) {
			ALS_ERROR(TEXT("GetWorld() called failed: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		const APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
		if (!PlayerCameraManager) {
			ALS_ERROR(TEXT("GetPlayerCameraManager() called failed: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UALSGameInstance* GameInstance = Cast<UALSGameInstance>(Gun->GetGameInstance());
		if (!GameInstance) {
			ALS_ERROR(TEXT("GameInstance is not ALSGameInstance: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Gun->WeaponAttackOptions.AttackCD = Gun->WeaponAttackOptions.AttackInterval;

		const FVector&& MuzzleLocation = Gun->MuzzleScene->GetComponentLocation();
		const FVector&& CameraLocation = PlayerCameraManager->GetCameraLocation();
		const FRotator&& CameraRotation = PlayerCameraManager->GetCameraRotation();
		const FVector&& CameraForward = CameraRotation.Vector();
		const float Offset = FMath::Abs(CameraRotation.UnrotateVector(CameraLocation).X - CameraRotation.UnrotateVector(MuzzleLocation).X);
		const FVector Start = CameraLocation + CameraForward * Offset;
		FVector End = Start + CameraForward * Gun->WeaponAttackOptions.Range;

		// 子弹计算和屏显
		FALSAmmoInfo& AmmoInfo = Gun->AmmoInfo;
		if (AmmoInfo.CurrentAmmo <= 0) {
			if (Gun->EmptySound) {
				UGameplayStatics::SpawnSoundAtLocation(World, Gun->EmptySound, MuzzleLocation);
			}
			break;
		}

		AmmoInfo.CurrentAmmo--;
		UALSPlayerHUD* HUD = Character->GetHUD();
		if (HUD) {
			HUD->SetCurrentAmmo(AmmoInfo.CurrentAmmo);
		}

		// 播放音效
		if (Gun->WeaponEffectsOptions.AttackSound) {
			UGameplayStatics::SpawnSoundAtLocation(World, Gun->WeaponEffectsOptions.AttackSound, MuzzleLocation);
		}

		// 播放特效
		if (Gun->WeaponEffectsOptions.AttackEffects) {
			UGameplayStatics::SpawnEmitterAttached(Gun->WeaponEffectsOptions.AttackEffects, Gun->MuzzleScene);
		}

		// 播放攻击蒙太奇
		if (Gun->WeaponAnimationOptions.AttackMontage) {
			Character->PlayAnimMontage(Gun->WeaponAnimationOptions.AttackMontage);
		}

		// 射线检测
		FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), false);
		TraceParams.AddIgnoredActor(Character);
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult OutHitResult;
		World->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Camera, TraceParams);

#if ENABLE_DRAW_DEBUG
		DrawDebugLineTraceSingle(World, Start, End, EDrawDebugTrace::Type(DebugTrace),
			OutHitResult.bBlockingHit, OutHitResult, FLinearColor::Red, FLinearColor::Green, 5.f);
#endif

		bool bTracer = true;

		if (OutHitResult.bBlockingHit && !OutHitResult.bStartPenetrating) {
			End = OutHitResult.ImpactPoint;
			const FRotator&& EndRotation = OutHitResult.Normal.ToOrientationRotator();

			if (OutHitResult.Distance < 200) {
				// 如果距离2米以内就不显示Tracer.
				bTracer = false;
			}

			if (Gun->MarkerClass) {
				GameInstance->ALSActorPool->Get(World, Gun->MarkerClass, End, EndRotation, 5.f);
			}

			if (OutHitResult.PhysMaterial.IsValid()) {
				// 播放环境特效
				AALSGameMode* ALSGameMode = UALSLibrary::Instance()->GetGameMode(World);
				if (!ALSGameMode) {
					break;
				}
				ALSGameMode->PlayEffect(OutHitResult.PhysMaterial->SurfaceType, End, EndRotation, Gun->WeaponEffectsOptions.HittedEffects);
			}

			AALSBaseCharacter* HitCharacter = Cast<AALSBaseCharacter>(OutHitResult.GetActor());
			// TODO: 这里需要为 AALSBaseCharacter 添加阵营, 不同阵营才会受到伤害
			if (HitCharacter/* && HitCharacter->ActorHasTag(TEXT("Enemy"))*/) {
				HitCharacter->Hit(Gun->WeaponAttackOptions.Damage);
			}
		}

		// 后坐力
		if (Gun->Recoil) {
			Character->AddControllerPitchInput(-FMath::FRandRange(0, Gun->Recoil));
		}

		// 子弹拖尾
		if (Gun->TracerClass && bTracer) {
			const FRotator&& MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, End);
			GameInstance->ALSActorPool->Get(World, Gun->TracerClass, MuzzleLocation, MuzzleRotation, 2.f);
		}

		// 装弹
		if (AmmoInfo.CurrentAmmo == 0 && AmmoInfo.MaxAmmo > 0) {
			Reload(Gun);
		}
	} while (0);
}

inline AWeaponBase*
UPropsComponent::GetCurrentWeapon(EALSOverlayState Overlay) const {
	switch (Overlay) {
	case EALSOverlayState::Rifle: return Rifle;
	case EALSOverlayState::PistolOneHanded: return Pistol;
	// case EALSOverlayState::PistolTwoHanded: break;
	// case EALSOverlayState::Bow: break;
	}

	return nullptr;
}
