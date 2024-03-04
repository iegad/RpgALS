#pragma once

#include "Bases/XEnum.h"
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "XGunData.generated.h"

class AXMagazine;
class AXMarker;
class AXTracer;

USTRUCT(BlueprintType)
struct FXGunData : public FTableRowBase {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Information")
	TObjectPtr<UStaticMesh> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Information")
	EXGunType GunType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Information")
	FString GunName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Information")
	FString Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Information")
	float Mass = 0.f;

	// 攻击间隔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Amounts")
	float FireInterval = 0.f;

	// CD
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "Xrs|Amounts")
	float FireCD = 0.f;

	// 射程
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Amounts")
	float Range = 0.f;

	// 伤害
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Amounts")
	float Damage = 0.f;

	// 后坐力
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Amounts")
	float Recoil = 0.f;

	// 弹匣上限
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Ammo")
	int32 ClipAmmo = 0;

	// 当前弹药数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Xrs|Ammo")
	int32 CurrentAmmo = 0;

	// 备用弹药数
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Xrs|Ammo")
	int32 ReserveAmmo = 0;

	// 子弹类型
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Ammo")
	EXBulletType BulletType;

	// 空弹匣
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Ammo")
	TSubclassOf<AXMagazine> MagazineEmpty;

	// 满弹匣
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Ammo")
	TSubclassOf<AXMagazine> MagazineFull;

	// 子弹Tracer
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Effect")
	TSubclassOf<AXTracer> BulletTracer;

	// 弹孔
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Effect")
	TSubclassOf<AXMarker> BulletHole;

	// 枪口特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Effect")
	TObjectPtr<UParticleSystem> MuzzleVFX;

	// 击中特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Effect")
	TObjectPtr<UParticleSystem> HittedVFX;

	// 射击音效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Effect")
	TObjectPtr<USoundBase> FireSFX;

	// 空枪音效
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Effect")
	TObjectPtr<USoundBase> EmptyFireSFX;

	// 拾取武器动画
	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Montage")
	TArray<TObjectPtr<UAnimMontage>> PickupMontages;*/

	// 丢弃武器
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Montage")
	TObjectPtr<UAnimMontage> DropMontage;

	// 装备武器动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Montage")
	TObjectPtr<UAnimMontage> EquipMontage;

	// 卸载武器动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Montage")
	TObjectPtr<UAnimMontage> UnequipMontage;

	// 射击动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Montage")
	TObjectPtr<UAnimMontage> FireMontage;

	// 换弹匣动画
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Montage")
	TObjectPtr<UAnimMontage> ReloadMontage;

	// 装备插槽, 手上的插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Sockets")
	FName EquipSocket;

	// 枪口插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Sockets")
	FName MuzzleSocket;

	// 弹匣插槽, 枪上插槽
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|Sockets")
	FName MagazineSocket;
};