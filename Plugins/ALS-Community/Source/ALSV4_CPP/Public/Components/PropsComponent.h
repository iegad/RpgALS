// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "Library/ALSCharacterEnumLibrary.h"
#include "Props/Weapons/Guns/GunBase.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PropsComponent.generated.h"

class AALSBaseCharacter;
class APropsBase;
class AWeaponBase;
class AGunBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UPropsComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UPropsComponent();

	void StartEquip(APropsBase* Props) const;
	void EndEquip(APropsBase* Props) const;
	void EndReload(AGunBase* Gun) const;
	void StartUnEquip(APropsBase* Props) const;
	void EndUnEquip(APropsBase* Props) const;

public:
	UFUNCTION(BlueprintCallable)
	void Equip(APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void UnEquip(APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void Attack(AWeaponBase* Weapon, int DebugTrace) const;

	UFUNCTION(BlueprintCallable)
	void Reload(AGunBase* Gun) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<AGunBase> Rifle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<AGunBase> Pistol;

protected:
	void AttackInternal(AWeaponBase* Weapon, int DebugTrace) const;
	void AttackInternal(AGunBase* Gun, int DebugTrace) const;

private:
	inline AWeaponBase* GetCurrentWeapon(EALSOverlayState Overlay) const;
	inline AALSBaseCharacter* GetALSBaseCharacter() const;

	mutable APropsBase* DesiredProps = nullptr;
};
