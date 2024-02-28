// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "Props/PropsBase.h"
#include "WeaponBase.generated.h"

class UAnimMontage;
class AALSBaseCharacter;

USTRUCT(BlueprintType)
struct FWeaponAttackOptions {
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float AttackInterval = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Range = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Damage = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float AttackCD = 0.f;

	bool CheckInitialized() const;
};

USTRUCT(BlueprintType)
struct FWeaponEffectsOptions {
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> AttackSound;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UParticleSystem> AttackEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UParticleSystem> HittedEffects;

	bool CheckInitialized() const;
};

USTRUCT(BlueprintType)
struct FWeaponAnimationOptions {
	GENERATED_BODY();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> AttackMontage;

	bool CheckInitialized() const;
};

UCLASS()
class ALSV4_CPP_API AWeaponBase : public APropsBase {
	GENERATED_BODY()
	
public:
	AWeaponBase();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponAttackOptions WeaponAttackOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponEffectsOptions WeaponEffectsOptions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FWeaponAnimationOptions WeaponAnimationOptions;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
};
