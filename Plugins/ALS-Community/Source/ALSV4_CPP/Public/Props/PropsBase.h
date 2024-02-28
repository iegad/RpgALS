// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "Library/ALSCharacterEnumLibrary.h"

#include "CoreMinimal.h"
#include "ALSActor.h"
#include "PropsBase.generated.h"

UCLASS()
class ALSV4_CPP_API APropsBase : public AALSActor {
	GENERATED_BODY()
	
public:
	APropsBase();

	inline void Lock() { Locked = true; }
	inline void UnLock() { Locked = false; }
	bool GetLock() const { return Locked; }

public:
	/// <summary>
	/// 根组件
	/// </summary>
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	/// <summary>
	/// 道具骨骼网格体
	/// </summary>
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> UnEquipMontage;

	/// <summary>
	/// 道具的叠加态Tag
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EALSOverlayState OverlayState;

	/// <summary>
	/// 装备道具的骨骼名称
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName EquipSocketName;

protected:
	virtual void BeginPlay() override;


private:
	bool Locked = false;
};
