#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ALSLibrary.generated.h"

#define ALS_LOG(Format, ...) UE_LOG(LogTemp, Display, Format, ##__VA_ARGS__)
#define ALS_WARN(Format, ...) UE_LOG(LogTemp, Warning, Format, ##__VA_ARGS__)
#define ALS_ERROR(Format, ...) UE_LOG(LogTemp, Error, Format, ##__VA_ARGS__)
#define ALS_FATAL(Format, ...) UE_LOG(LogTemp, Fatal, Format, ##__VA_ARGS__)

const FString GameSettingsWindowMode_Windowed{ TEXT("Windowed") };
const FString GameSettingsWindowMode_FullScreen{ TEXT("Full Screen") };
const FString GameSettingsWindowMode_WindowedFullScreen{ TEXT("Windowed Full Screen") };
const FString GameSettingsFrameRateLimit_60{ TEXT("FPS 60") };
const FString GameSettingsFrameRateLimit_90{ TEXT("FPS 90") };
const FString GameSettingsFrameRateLimit_120{ TEXT("FPS 120") };
const FString GameSettingsFrameRateLimit_Infinite{ TEXT("Infinite") };

class UInputAction;
class UInputMappingContext;
class UALSGameInstance;
class AALSGameMode;

USTRUCT(BlueprintType)
struct FPlayerInputSystem {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Aim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_AttackHold;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_AttackTap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Crouch;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Move;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Jump;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Pistol;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Ragdoll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Rifle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Roll;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Settings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Sprint;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Walk;
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

USTRUCT(BlueprintType)
struct FCharacterStateSystem {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHP = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HP = 0.f;
};

UENUM(BlueprintType)
enum class EFrameRateLimit : uint8 {
	Infinite = 0,
	FPS60,
	FPS90,
	FPS120,
};

USTRUCT(BlueprintType)
struct FGameSettings {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ShowFPS = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FIntPoint Resolution{1920, 1080};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EWindowMode::Type> WindowMode{ EWindowMode::Windowed };

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EFrameRateLimit FrameRateLimit{ EFrameRateLimit::Infinite };

	void Reset();

	void SetResolution(const FString& Value);
	void SetWindowMode(const FString& Value);
	void SetFrameRateLimit(const FString& Value);

	float GetFrameRateLimitValue() const;
	FString GetResolutionString() const;
	FString GetWindowModeString() const;
	FString GetFrameRateLimitString() const;
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

	UWorld* GetCurrentWorld() const;
	UALSGameInstance* GetGameInstance(UWorld* World = nullptr) const;
	AALSGameMode* GetGameMode(UWorld* World = nullptr) const;
	
private:
	UALSLibrary() : Super() {}
};
