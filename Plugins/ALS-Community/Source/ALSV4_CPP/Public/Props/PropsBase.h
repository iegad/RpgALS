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
	/// �����
	/// </summary>
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	/// <summary>
	/// ���߹���������
	/// </summary>
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Montage")
	TObjectPtr<UAnimMontage> EquipMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Montage")
	TObjectPtr<UAnimMontage> UnEquipMontage;

	/// <summary>
	/// ���ߵĵ���̬Tag
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS")
	EALSOverlayState OverlayState;

	/// <summary>
	/// װ�����ߵĹ�������
	/// </summary>
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS")
	FName EquipSocketName;

protected:
	virtual void BeginPlay() override;
};
