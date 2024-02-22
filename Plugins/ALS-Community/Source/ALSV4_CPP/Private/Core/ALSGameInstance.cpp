// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Core/ALSGameInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSGameInstance)

#include "GameFramework/GameUserSettings.h"

#include "ALSLibrary.h"
#include "ALSActor.h"

void
UALSGameInstance::QuitGame() {
#if not WITH_EDITOR 
	RequestEngineExit(TEXT("Exit"));
#endif // WITH_EDITOR 
}

UALSGameInstance::UALSGameInstance() : Super() {
	ALSActorPool = CreateDefaultSubobject<UALSActorPool>(TEXT("ALSActorPool"));
	ALSActorPool->Init(this);
}

void 
UALSGameInstance::LoadGameSettings() {
	SaveGameSettings(PlayerGameSettings);
}

void
UALSGameInstance::SaveGameSettings(const FGameSettings& NewGameSettings) {
	SaveGraphicsSettings(NewGameSettings.GraphicsSettings);
}

void 
UALSGameInstance::ResetGameSettings() {
	SaveGameSettings(FGameSettings::DefaultGameSettings);
}

void 
UALSGameInstance::SaveGraphicsSettings(const FGraphicsSettings& NewGraphicsSettings) {
	PlayerGameSettings.GraphicsSettings = NewGraphicsSettings;
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();

	if (!GameUserSettings) {
		ALS_FATAL(TEXT("GEngine->GetGameUserSettings() called failed: %s:%d"), __FILEW__, __LINE__);
		return;
	}
	
	GameUserSettings->SetScreenResolution(NewGraphicsSettings.Resolution);
	GameUserSettings->SetFullscreenMode(NewGraphicsSettings.WindowMode);
	GameUserSettings->SetFrameRateLimit(NewGraphicsSettings.GetFrameRateLimitValue());
	GameUserSettings->SetOverallScalabilityLevel((int32)NewGraphicsSettings.GraphicsQuality);

	if (NewGraphicsSettings.GraphicsQuality == EGraphicsQuality::Custom) {
		GameUserSettings->SetReflectionQuality((int32)NewGraphicsSettings.ReflectionQuality);
		GameUserSettings->SetPostProcessingQuality((int32)NewGraphicsSettings.PostProcessingQuality);
		GameUserSettings->SetAntiAliasingQuality((int32)NewGraphicsSettings.AntiAliasingQuality);
		GameUserSettings->SetGlobalIlluminationQuality((int32)NewGraphicsSettings.GlobalIlluminationQuality);
		GameUserSettings->SetVisualEffectQuality((int32)NewGraphicsSettings.VisualEffectQuality);
		GameUserSettings->SetViewDistanceQuality((int32)NewGraphicsSettings.ViewDistanceQuality);
		GameUserSettings->SetTextureQuality((int32)NewGraphicsSettings.TextureQuality);
		GameUserSettings->SetShadowQuality((int32)NewGraphicsSettings.ShadowQuality);
		GameUserSettings->SetShadingQuality((int32)NewGraphicsSettings.ShadingQuality);
		GameUserSettings->SetFoliageQuality((int32)NewGraphicsSettings.FoliageQuality);
	}

	GameUserSettings->SaveSettings();
	GameUserSettings->ApplySettings(true);
	GameUserSettings->ApplyResolutionSettings(true);
}

void 
UALSGameInstance::SaveGraphicsSettings(
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
	float FoliageQuality) {

	FGraphicsSettings NewGraphicsSettings{ .VSync = NewVSync, .ShowFPS = NewShowFPS };
	NewGraphicsSettings.SetResolution(NewResolution);
	NewGraphicsSettings.SetWindowMode(NewWindowMode);
	NewGraphicsSettings.SetFrameRateLimit(NewFrameRateLimit);
	NewGraphicsSettings.SetGraphicQuality(NewGraphicsQuality);

	if (NewGraphicsQuality == FGraphicsSettings::GraphicsSettingsGraphicsQuality_Custom) {
		NewGraphicsSettings.SetReflectionQuality(ReflectionQuality);
		NewGraphicsSettings.SetPostProcessingQuality(PostProcessingQuality);
		NewGraphicsSettings.SetAntiAliasingQuality(AntiAliasingQuality);
		NewGraphicsSettings.SetGlobalIlluminationQuality(GlobalIlluminationQuality);
		NewGraphicsSettings.SetVisualEffectQuality(VisualEffectQuality);
		NewGraphicsSettings.SetViewDistanceQuality(ViewDistanceQuality);
		NewGraphicsSettings.SetTextureQuality(TextureQuality);
		NewGraphicsSettings.SetShadowQuality(ShadowQuality);
		NewGraphicsSettings.SetShadingQuality(ShadingQuality);
		NewGraphicsSettings.SetFoliageQuality(FoliageQuality);
	}

	SaveGraphicsSettings(NewGraphicsSettings);
}