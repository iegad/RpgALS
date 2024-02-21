// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community
// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

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
class ALSV4_CPP_API AALSCharacter : public AALSBaseCharacter {
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

protected:
	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	virtual void OnOverlayStateChanged(EALSOverlayState PreviousState) override;

	/** Implement on BP to update animation states of held objects */
	UFUNCTION(BlueprintCallable, Category = "ALS|HeldObject")
	void UpdateHeldObjectAnimations();

public:
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

private:
	bool bNeedsColorReset = false;
};
