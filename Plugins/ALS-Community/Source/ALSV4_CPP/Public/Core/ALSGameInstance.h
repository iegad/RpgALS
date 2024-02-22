// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "ALSLibrary.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ALSGameInstance.generated.h"

class UALSActorPool;

UCLASS()
class ALSV4_CPP_API UALSGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|ALSActor Pool")
	TObjectPtr<UALSActorPool> ALSActorPool;

	UFUNCTION(BlueprintCallable, Category = "ALS|Game Settings")
	void QuitGame();
	
public:
	UALSGameInstance();
	const FGameSettings& GetPlayerGameSettings() { return PlayerGameSettings; }

public:
	UFUNCTION(BlueprintCallable, Category = "ALS|Game Settings")
	void LoadGameSettings();

	UFUNCTION(BlueprintCallable, Category = "ALS|Game Settings")
	void SaveGameSettings(const FGameSettings& NewGameSettings);

	UFUNCTION(BlueprintCallable, Category = "ALS|Game Settings")
	void ResetGameSettings();

	UFUNCTION(BlueprintCallable, Category = "ALS|Game Settings")
	void SaveGraphicsSettings(const FGraphicsSettings& NewGraphicsSettings);

public:
	void SaveGraphicsSettings(
		bool NewVSync,
		bool NewShowFPS, 
		const FString& NewResolution, 
		const FString& NewWindowMode, 
		const FString& NewFrameRateLimit,
		const FString& NewGraphicsQuality,
		float ReflectionQuality,
		float PostProcessingQuality,
		float AntiAliasingQuality,
		float GlobalIlluminationQuality,
		float VisualEffectQuality,
		float ViewDistanceQuality,
		float TextureQuality,
		float ShadowQuality,
		float ShadingQuality,
		float FoliageQuality);

private:
	FGameSettings PlayerGameSettings;
};
