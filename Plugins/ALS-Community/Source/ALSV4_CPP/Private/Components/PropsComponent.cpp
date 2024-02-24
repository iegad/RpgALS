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
#include "Props/Weapons/MarkerBase.h"
#include "Props/Weapons/TracerBase.h"
#include "Core/ALSGameMode.h"
#include "UI/ALSPlayerHUD.h"

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
		Props->UnLock();
		AGunBase* Gun = Cast<AGunBase>(Props);
		UALSPlayerHUD* HUD = Character->GetHUD();
		if (Gun && HUD) {
			HUD->ShowRifleAmmo(35, 245/*Gun->Ammo, GetAmmo(Props->OverlayState)*/);
		}
	} while (0);
}

void 
UPropsComponent::EndReload(AGunBase* Gun) const {
	if (!Gun) {
		return;
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
			break;;
		}

		AALSBaseCharacter* Character = GetALSBaseCharacter();
		if (!Character) {
			ALS_ERROR(TEXT("UPropsComponent's GetOwner is not AALSCharacter: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		Props->UnLock();
		Character->SetOverlayState(EALSOverlayState::Default);

		if (DesiredProps) {
			Equip(DesiredProps);
		}

		AGunBase* Gun = Cast<AGunBase>(Props);
		UALSPlayerHUD* HUD = Character->GetHUD();
		if (Gun && HUD) {
			HUD->HideRifleAmmo();
		}
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
			ALS_WARN(TEXT("ReloadMontage is nullptr: %s:%d"), __FILEW__, __LINE__);
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

		const FVector&& MuzzleLocation = Gun->Mesh->GetSocketLocation(Gun->MuzzleSocketName);
		const FVector&& CameraLocation = PlayerCameraManager->GetCameraLocation();
		const FRotator&& CameraRotation = PlayerCameraManager->GetCameraRotation();
		const FVector&& CameraForward = CameraRotation.Vector();
		const float Offset = FMath::Abs(CameraRotation.UnrotateVector(CameraLocation).X - CameraRotation.UnrotateVector(MuzzleLocation).X);
		const FVector Start = CameraLocation + CameraForward * Offset;
		FVector End = Start + CameraForward * Gun->WeaponAttackOptions.Range;

		// ������Ч
		if (Gun->WeaponEffectsOptions.AttackSound) {
			UGameplayStatics::SpawnSoundAtLocation(World, Gun->WeaponEffectsOptions.AttackSound, MuzzleLocation);
		}

		// ������Ч
		if (Gun->WeaponEffectsOptions.AttackEffects) {
			UGameplayStatics::SpawnEmitterAttached(Gun->WeaponEffectsOptions.AttackEffects, Gun->MuzzleScene);
		}

		// ���Ź�����̫��
		if (Gun->WeaponAnimationOptions.AttackMontage) {
			Character->PlayAnimMontage(Gun->WeaponAnimationOptions.AttackMontage);
		}

		// ���߼��
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
				// �������2�����ھͲ���ʾTracer.
				bTracer = false;
			}

			if (Gun->MarkerClass) {
				GameInstance->ALSActorPool->Get(World, Gun->MarkerClass, End, EndRotation, 5.f);
			}

			if (OutHitResult.PhysMaterial.IsValid()) {
				// ���Ż�����Ч
				AALSGameMode* ALSGameMode = UALSLibrary::Instance()->GetGameMode(World);
				if (!ALSGameMode) {
					break;
				}
				ALSGameMode->PlayEffect(OutHitResult.PhysMaterial->SurfaceType, End, EndRotation, Gun->WeaponEffectsOptions.HittedEffects);
			}

			AALSBaseCharacter* HitCharacter = Cast<AALSBaseCharacter>(OutHitResult.GetActor());
			// TODO: ������ҪΪ AALSBaseCharacter �����Ӫ, ��ͬ��Ӫ�Ż��ܵ��˺�
			if (HitCharacter/* && HitCharacter->ActorHasTag(TEXT("Enemy"))*/) {
				HitCharacter->Hit(Gun->WeaponAttackOptions.Damage);
			}
		}

		// ������
		if (Gun->Recoil) {
			Character->AddControllerPitchInput(-FMath::FRandRange(0, Gun->Recoil));
		}

		if (Gun->TracerClass && bTracer) {
			const FRotator&& MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, End);
			GameInstance->ALSActorPool->Get(World, Gun->TracerClass, MuzzleLocation, MuzzleRotation, 2.f);
		}
	} while (0);
}

inline int32 
UPropsComponent::GetAmmo(EALSOverlayState Overlay) const {
	switch (Overlay) {
	case EALSOverlayState::Rifle: return RifleAmmo;
	case EALSOverlayState::PistolOneHanded: return PistolAmmo;
	// case EALSOverlayState::PistolTwoHanded: break;
	// case EALSOverlayState::Bow: break;
	}

	return -1;
}
