#include "Props/Weapons/Guns/GunBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GunBase)

#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"
#include "ALSLibrary.h"
#include "Character/ALSCharacter.h"

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

		WeaponAttackOptions.AttackCD = WeaponAttackOptions.AttackInterval;
		const FVector&& MuzzleLocation = Mesh->GetSocketLocation(MuzzleSocketName);
		const FRotator&& MuzzleRotation = Mesh->GetSocketRotation(MuzzleSocketName);
		const FVector&& CameraLocation = PlayerCameraManager->GetCameraLocation();
		const FRotator&& CameraRotation = PlayerCameraManager->GetCameraRotation();
		const FVector&& CameraForward = CameraRotation.Vector();
		const float Offset = FMath::Abs(CameraRotation.UnrotateVector(CameraLocation).X - CameraRotation.UnrotateVector(MuzzleLocation).X);
		const FVector Start = CameraLocation + CameraForward * Offset;
		FVector End = Start + CameraForward * WeaponAttackOptions.Range;

		FCollisionQueryParams TraceParams(TEXT("WeaponTrace"), false);
		TraceParams.AddIgnoredActor(Character);
		TraceParams.bReturnPhysicalMaterial = true;

		FHitResult OutHitResult;
		World->LineTraceSingleByChannel(OutHitResult, Start, End, ECollisionChannel::ECC_Camera);

#if ENABLE_DRAW_DEBUG
		DrawDebugLineTraceSingle(World, Start, End, EDrawDebugTrace::Type(DebugTrace),
			OutHitResult.bBlockingHit, OutHitResult, FLinearColor::Red, FLinearColor::Green, 5.f);
#endif
	} while (0);
}

void
AGunBase::BeginPlay() {
	Super::BeginPlay();

	if (MuzzleSocketName.IsNone()) {
		ALS_ERROR(TEXT("MuzzleSocketName is invalid: %s:%d"), __FILEW__, __LINE__);
	}
}
