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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Montage")
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Montage")
	TObjectPtr<UAnimMontage> UnEquipMontage;

	/// <summary>
	/// 道具的叠加态Tag
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS")
	EALSOverlayState OverlayState;

	/// <summary>
	/// 装备道具的骨骼名称
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS")
	FName EquipSocketName;

protected:
	virtual void BeginPlay() override;
};
