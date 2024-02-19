#include "Core/ALSGameMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSGameMode)

#include "Sound/SoundBase.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

#include "ALSLibrary.h"
#include "Core/ALSPlayerController.h"

AALSGameMode::AALSGameMode() : Super() {
}

void 
AALSGameMode::PlayEffect(EPhysicalSurface Surface, const FVector& Location, const FVector& Normal, UParticleSystem* AdditiveVFX) {
	UWorld* World = GetWorld();
	if (!World) {
		return;
	}

	const EEnvironmentPhysicalMaterials PhysicalMaterial = EEnvironmentPhysicalMaterials(Surface);

	UParticleSystem* Particle = EnvironmentVFX.FindRef(PhysicalMaterial);
	USoundBase* Sound = EnvironmentSFX.FindRef(PhysicalMaterial);

	const FRotator&& Rotation = Normal.ToOrientationRotator();

	if (Sound) {
		UGameplayStatics::SpawnSoundAtLocation(World, Sound, Location, Rotation);
	}
	
	if (Particle) {
		UGameplayStatics::SpawnEmitterAtLocation(World, Particle, Location, Rotation);
	}

	if (AdditiveVFX) {
		UGameplayStatics::SpawnEmitterAtLocation(World, AdditiveVFX, Location, Rotation);
	}
}

void 
AALSGameMode::BeginPlay() {
	if (!EnvironmentSFX.Num()) {
		ALS_ERROR(TEXT("%s's EnvironmentSFX is not set: %s:%d"), *this->GetClass()->GetName(), __FILEW__, __LINE__);
	}

	if (!EnvironmentVFX.Num()) {
		ALS_ERROR(TEXT("%s's EnvironmentVFX is not set: %s:%d"), *this->GetClass()->GetName(), __FILEW__, __LINE__);
	}
}
