#pragma once

#include "Containers/Deque.h"

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ALSGameMode.generated.h"

class AALSActor;

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

public:
	AALSGameMode();

	void PlayEffect(EPhysicalSurface Surface, const FVector& Location, const FVector& Normal, UParticleSystem* AdditiveVFX = nullptr);
	AALSActor* GetALSActor(UClass* Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan);
	void PutALSActor(AALSActor* Actor);

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	void CheckActorLifeSpan();

	TDeque<AALSActor*> UnactivePool;
	TDoubleLinkedList<AALSActor*> ActivePool;
	FTimerHandle LifeSpanTimer;
};
