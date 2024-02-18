#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ALSLibrary.generated.h"

#define ALS_LOG(Format, ...) UE_LOG(LogTemp, Display, Format, ##__VA_ARGS__)
#define ALS_WARN(Format, ...) UE_LOG(LogTemp, Warning, Format, ##__VA_ARGS__)
#define ALS_ERROR(Format, ...) UE_LOG(LogTemp, Error, Format, ##__VA_ARGS__)

class UInputAction;
class UInputMappingContext;

USTRUCT(BlueprintType)
struct FPlayerInputSystem {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> AttackHoldAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> AttackTapAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> CameraHeldAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> CameraRightAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> CameraTapAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> CameraUpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> ForwardMovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> PistolAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> RagdollAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> RifleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> RightMovementAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> SprintAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> StanceAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> WalkAction;
};

USTRUCT(BlueprintType)
struct FDebugInputSystem {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugFocusedCharacterCycleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugOpenOverlayMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugOverlayMenuCycleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleCharacterInfoAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleMeshAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleDebugViewAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleHudAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleLayerColorsAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleShapesAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleSlomoAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleTracesAction;
};

UCLASS()
class ALSV4_CPP_API UALSLibrary : public UObject {
	GENERATED_BODY()

public:
	static UALSLibrary* Instance() {
		static UALSLibrary* instance = nullptr;

		if (!instance) {
			instance = NewObject<UALSLibrary>();
		}

		return instance;
	}
	
private:
	UALSLibrary() : Super() {}
};
