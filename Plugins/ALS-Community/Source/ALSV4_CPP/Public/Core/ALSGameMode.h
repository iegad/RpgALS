#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ALSGameMode.generated.h"

class UALSActorPoolComponent;

UENUM()
enum class EEnvironmentPhysicalMaterials {
	None,
	Rock,
	Wood,
	Grass,
	Metal,
	Glass,
	Flesh,
};

UCLASS()
class ALSV4_CPP_API AALSGameMode : public AGameMode {
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Effects")
	TMap<EEnvironmentPhysicalMaterials, USoundBase*> EnvironmentSFX {
		{EEnvironmentPhysicalMaterials::Rock, nullptr},
		{EEnvironmentPhysicalMaterials::Wood, nullptr},
		{EEnvironmentPhysicalMaterials::Grass, nullptr},
		{EEnvironmentPhysicalMaterials::Metal, nullptr},
		{EEnvironmentPhysicalMaterials::Glass, nullptr},
		{EEnvironmentPhysicalMaterials::Flesh, nullptr},
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Effects")
	TMap<EEnvironmentPhysicalMaterials, UParticleSystem*> EnvironmentVFX {
		{EEnvironmentPhysicalMaterials::Rock, nullptr},
		{EEnvironmentPhysicalMaterials::Wood, nullptr},
		{EEnvironmentPhysicalMaterials::Grass, nullptr},
		{EEnvironmentPhysicalMaterials::Metal, nullptr},
		{EEnvironmentPhysicalMaterials::Glass, nullptr},
		{EEnvironmentPhysicalMaterials::Flesh, nullptr},
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|ALSActor Pool")
	TObjectPtr<UALSActorPoolComponent> TracerPool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|ALSActor Pool")
	TObjectPtr<UALSActorPoolComponent> MarkerPool;

public:
	AALSGameMode();

	void PlayEffect(EPhysicalSurface Surface, const FVector& Location, const FRotator& Rotation, UParticleSystem* AdditiveVFX = nullptr);
	
protected:
	virtual void BeginPlay() override;
};
