#include "Props/Weapons/Guns/GunBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GunBase)

#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ALSLibrary.h"
#include "Character/ALSCharacter.h"
#include "Core/ALSGameInstance.h"
#include "Core/ALSGameMode.h"
#include "Props/Weapons/TracerBase.h"
#include "Props/Weapons/MarkerBase.h"

AGunBase::AGunBase() : Super() {
	MuzzleScene = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleScene"));
	MuzzleScene->SetupAttachment(GetRootComponent());
}

void 
AGunBase::Attack(AALSCharacter* Character, int DebugTrace) {
	do {
		if (!Character) {
			ALS_ERROR(TEXT("Character is nullptr: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (WeaponAttackOptions.Range <= 0.f) {
			ALS_ERROR(TEXT("%s Attack Range is 0: %s:%d"), *this->GetName(), __FILEW__, __LINE__);
			break;
		}

		if (WeaponAttackOptions.AttackCD > 0) {
			break;
		}

		UWorld* World = GetWorld();
		if (!World) {
			break;
		}

		const APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(World, 0);
		if (!PlayerCameraManager) {
			break;
		}

		UALSGameInstance* GameInstance = UALSLibrary::Instance()->GetGameInstance(World);

		WeaponAttackOptions.AttackCD = WeaponAttackOptions.AttackInterval;

		const FVector&& MuzzleLocation = Mesh->GetSocketLocation(MuzzleSocketName);
		const FVector&& CameraLocation = PlayerCameraManager->GetCameraLocation();
		const FRotator&& CameraRotation = PlayerCameraManager->GetCameraRotation();
		const FVector&& CameraForward = CameraRotation.Vector();
		const float Offset = FMath::Abs(CameraRotation.UnrotateVector(CameraLocation).X - CameraRotation.UnrotateVector(MuzzleLocation).X);
		const FVector Start = CameraLocation + CameraForward * Offset;
		FVector End = Start + CameraForward * WeaponAttackOptions.Range;

		// 播放音效
		if (WeaponEffectsOptions.AttackSound) {
			UGameplayStatics::SpawnSoundAtLocation(World, WeaponEffectsOptions.AttackSound, MuzzleLocation);
		}

		// 播放特效
		if (WeaponEffectsOptions.AttackEffects) {
			UGameplayStatics::SpawnEmitterAttached(WeaponEffectsOptions.AttackEffects, MuzzleScene);
		}

		// 播放攻击蒙太奇
		if (WeaponAnimationOptions.AttackMontage) {
			Character->PlayAnimMontage(WeaponAnimationOptions.AttackMontage);
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

			if (MarkerClass && GameInstance) {
				GameInstance->ALSActorPool->Get(World, MarkerClass, End, EndRotation, 5.f);
			}

			if (OutHitResult.PhysMaterial.IsValid()) {
				// 播放环境特效
				AALSGameMode* ALSGameMode = UALSLibrary::Instance()->GetGameMode(World);
				if (!ALSGameMode) {
					break;
				}
				ALSGameMode->PlayEffect(OutHitResult.PhysMaterial->SurfaceType, End, EndRotation, WeaponEffectsOptions.HittedEffects);
			}

			AALSCharacter* HitCharacter = Cast<AALSCharacter>(OutHitResult.GetActor());
			// TODO: 这里需要为 AALSBaseCharacter 添加阵营, 不同阵营才会受到伤害
			if (HitCharacter/* && HitCharacter->ActorHasTag(TEXT("Enemy"))*/) {
				HitCharacter->Hit(WeaponAttackOptions.Damage);
			}
		}

		// 后坐力
		if (Recoil) {
			Character->AddControllerPitchInput(-FMath::FRandRange(0, Recoil));
		}
		
		if (TracerClass && bTracer && GameInstance) {
			const FRotator&& MuzzleRotation = UKismetMathLibrary::FindLookAtRotation(MuzzleLocation, End);
			GameInstance->ALSActorPool->Get(World, TracerClass, MuzzleLocation, MuzzleRotation, 2.f);
		}
	} while (0);
}

void
AGunBase::BeginPlay() {
	Super::BeginPlay();

	if (MuzzleSocketName.IsNone()) {
		ALS_ERROR(TEXT("MuzzleSocketName is invalid: %s:%d"), __FILEW__, __LINE__);
	}
}
