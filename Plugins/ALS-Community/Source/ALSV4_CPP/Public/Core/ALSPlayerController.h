#pragma once

#include "ALSLibrary.h"

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"
#include "ALSPlayerController.generated.h"

class AALSBaseCharacter;
class UInputMappingContext;
class UALSGameSettings;

/**
 * Player controller class
 */
UCLASS(Blueprintable, BlueprintType)
class ALSV4_CPP_API AALSPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	virtual void OnPossess(APawn* NewPawn) override;
	virtual void OnRep_Pawn() override;
	virtual void SetupInputComponent() override;

protected:
	void SetupInputs();
	void SetupCamera();

	UFUNCTION(BlueprintCallable)
	void SetGlobalTimeDilationLocal(float Dilation);

	void IA_Move(const FInputActionValue& Value);
	void IA_Look(const FInputActionValue& Value);
	void IA_Jump(const FInputActionValue& Value);
	void IA_Sprint(const FInputActionValue& Value);
	void IA_Aim();
	void IA_Crouch();
	void IA_Walk();
	void IA_Ragdoll();
	void IA_Rifle();
	void IA_Roll();
	void IA_Pistol();
	void IA_AttackHold();
	void IA_AttackTap();
	void IA_Settings();

	// Debug actions
	void DebugToggleHudAction(const FInputActionValue& Value);
	void DebugToggleDebugViewAction(const FInputActionValue& Value);
	void DebugToggleTracesAction(const FInputActionValue& Value);
	void DebugToggleShapesAction(const FInputActionValue& Value);
	void DebugToggleLayerColorsAction(const FInputActionValue& Value);
	void DebugToggleCharacterInfoAction(const FInputActionValue& Value);
	void DebugToggleSlomoAction(const FInputActionValue& Value);
	void DebugFocusedCharacterCycleAction(const FInputActionValue& Value);
	void DebugToggleMeshAction(const FInputActionValue& Value);
	void DebugOpenOverlayMenuAction(const FInputActionValue& Value);
	void DebugOverlayMenuCycleAction(const FInputActionValue& Value);

public:
	/** Main character reference */
	UPROPERTY(BlueprintReadOnly, Category = "ALS")
	TObjectPtr<AALSBaseCharacter> PossessedCharacter = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player Input System")
	FPlayerInputSystem PlayerInputSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Debug Input System")
	FDebugInputSystem DebugInputSystem;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UALSGameSettings> ALSGameSettingsClass;

private:
	TObjectPtr<UALSGameSettings> UI_ALSGameSettings;
};
