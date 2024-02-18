// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#pragma once

#include "CoreMinimal.h"
#include "Character/ALSBaseCharacter.h"
#include "ALSCharacter.generated.h"

class APropsBase;
class UWeaponComponent;
/**
 * Specialized character class, with additional features like held object etc.
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSCharacter : public AALSBaseCharacter
{
	GENERATED_BODY()

public:
	AALSCharacter(const FObjectInitializer& ObjectInitializer);

	/** Implemented on BP to update held objects */
	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void ClearHeldObject();

	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, class UClass* NewAnimClass, bool bLeftHand, FVector Offset);

	virtual void RagdollStart() override;

	virtual void RagdollEnd() override;

	virtual ECollisionChannel GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius) override;

	virtual FTransform GetThirdPersonPivotTarget() override;

	virtual FVector GetFirstPersonCameraTarget() override;

	APropsBase* GetCurrentProps() const;

	APropsBase* GetPropsFromOverlayState(EALSOverlayState Overlay) const;

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	virtual void RifleAction_Implementation() override;

	virtual void PistolAction_Implementation() override;

	virtual void AttackHoldAction_Implementation() override;

	virtual void AttackTapAction_Implementation() override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObjectAnimations();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USceneComponent> HeldObjectRoot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ALS|Component")
	TObjectPtr<UStaticMeshComponent> StaticMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|HeldObject")
	TMap<EALSOverlayState, USkeletalMesh*> SkeletalMeshMap{
		{EALSOverlayState::Rifle, nullptr},
		{EALSOverlayState::PistolOneHanded, nullptr},
		{EALSOverlayState::PistolTwoHanded, nullptr},
		{EALSOverlayState::Bow, nullptr},
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "ALS|HeldObject")
	TMap<EALSOverlayState, UStaticMesh*> StaticMeshMap{
		{EALSOverlayState::Torch, nullptr},
		{EALSOverlayState::Binoculars, nullptr},
		{EALSOverlayState::Box, nullptr},
		{EALSOverlayState::Barrel, nullptr},
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|HeldObject")
	TMap<EALSOverlayState, TSubclassOf<UAnimInstance>> AnimClassMap{ {EALSOverlayState::Bow, nullptr} };

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Props System")
	TObjectPtr<USceneComponent> SceneRifle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Props System")
	TObjectPtr<UChildActorComponent> ChildActorRifle;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Props System")
	TObjectPtr<USceneComponent> ScenePistol;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Props System")
	TObjectPtr<UChildActorComponent> ChildActorPistol;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "ALS|Props System")
	TObjectPtr<UWeaponComponent> WeaponComponent;

private:
	inline void CreatePropsSystem();
	inline void CreateCustomComponent();

private:
	bool bNeedsColorReset = false;
};
