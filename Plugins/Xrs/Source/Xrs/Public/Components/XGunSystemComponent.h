// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include "Bases/XEnum.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XGunSystemComponent.generated.h"

class AXBaseGun;
class AXMagazine;
class UPhysicsConstraintComponent;
class UStaticMeshComponents;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPickupDeletegate, AXBaseGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDropDeletegate, AXBaseGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FEquipDeletegate, AXBaseGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnequipDeletegate, AXBaseGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFocusGunDelegate, AXBaseGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUnfocusGunDelegate, AXBaseGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FReloadDelegate, AXBaseGun*, Gun);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FObstacleDelegate, AActor*, HitActor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XRS_API UXGunSystemComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UXGunSystemComponent();

	UFUNCTION(BlueprintCallable)
	void CheckObstacle(float Limit = 10.f);

	UFUNCTION(BlueprintCallable)
	void Pickup(AXBaseGun* Gun);

	UFUNCTION(BlueprintCallable)
	void Drop(int32 index);

	UFUNCTION(BlueprintCallable)
	void Equip(int32 index);
	void ANS_StartEquip(int32 index);
	void ANS_EndEquip(int32 index);

	UFUNCTION(BlueprintCallable)
	void Unequip();
	void ANS_StartUnequip();
	void ANS_EndUnequip();

	UFUNCTION(BlueprintCallable)
	void Fire();

	UFUNCTION(BlueprintCallable)
	void Reload();
	void ANS_StartRealod();
	void ANS_EndReload();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE AXBaseGun* GetGun(int32 index = 0) const { check(index >= 0 && index < Guns.Num());  return Guns.IsEmpty() ? nullptr : Guns[index]; }

	UPROPERTY(BlueprintAssignable)
	FObstacleDelegate OnObstacle;

	UPROPERTY(BlueprintAssignable)
	FPickupDeletegate OnPickup;

	UPROPERTY(BlueprintAssignable);
	FDropDeletegate OnDrop;

	UPROPERTY(BlueprintAssignable);
	FEquipDeletegate OnEquip;

	UPROPERTY(BlueprintAssignable);
	FUnequipDeletegate OnUnequip;

	UPROPERTY(BlueprintAssignable);
	FFocusGunDelegate OnFocusGun;

	UPROPERTY(BlueprintAssignable);
	FUnfocusGunDelegate OnUnfocusGun;

	UPROPERTY(BlueprintAssignable);
	FReloadDelegate OnReload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Names")
	FName NameRifleAttachMesh;

protected:
	virtual void BeginPlay() override;

private:
	int32 CurrentGunIndex = -1;
	int32 DesiredEquipGunIndex = -1;
	TObjectPtr<ACharacter> Character;
	TObjectPtr<UStaticMeshComponent> RifleAttachMesh;
	TObjectPtr<AXMagazine> MagazineEmpty;
	TObjectPtr<AXMagazine> MagazineFull;
	TArray<AXBaseGun*> Guns{ nullptr, nullptr, nullptr, nullptr };
};
