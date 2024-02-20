#pragma once

#include "CoreMinimal.h"
#include "Props/Weapons/WeaponBase.h"
#include "GunBase.generated.h"

class ATracerBase;
class AMarkerBase;

UCLASS()
class ALSV4_CPP_API AGunBase : public AWeaponBase {
	GENERATED_BODY()
	
public:
	AGunBase();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> MuzzleScene;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ATracerBase> TracerBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMarkerBase> MarkerBase;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Recoil = 0.f;

public:
	virtual void Attack(AALSCharacter* Character, int DebugTrace) override;

protected:
	virtual void BeginPlay() override;
};
