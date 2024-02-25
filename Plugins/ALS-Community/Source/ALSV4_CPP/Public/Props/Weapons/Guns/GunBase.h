// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "Props/Weapons/WeaponBase.h"
#include "GunBase.generated.h"

class ATracerBase;
class AMarkerBase;

USTRUCT(BlueprintType)
struct FALSAmmoInfo {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 CurrentAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxAmmo = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ClipAmmo = 0;
};

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
	TSubclassOf<ATracerBase> TracerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AMarkerBase> MarkerClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Recoil = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ReloadMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> EmptySound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FALSAmmoInfo AmmoInfo;

protected:
	virtual void BeginPlay() override;
};