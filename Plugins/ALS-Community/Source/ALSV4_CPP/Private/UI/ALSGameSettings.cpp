// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "UI/ALSGameSettings.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSGameSettings)

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "Components/ScrollBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

#include "Core/ALSGameInstance.h"

void 
UALSGameSettings::BTN_ConfirmHandler() {
	UALSGameInstance* GameInstance = Cast<UALSGameInstance>(GetGameInstance());
	if (!GameInstance) {
		ALS_ERROR(TEXT("Get ALSGameInstance failed: %s:%d"), __FILEW__, __LINE__);
		return;
	}

	GameInstance->SaveGraphicsSettings(
		CB_VSync->IsChecked(),
		CB_ShowFPS->IsChecked(), 
		DDL_Resolutions->GetSelectedOption(),
		DDL_WindowModes->GetSelectedOption(),
		DDL_FrameRateLimits->GetSelectedOption(), 
		DDL_GraphicsQuality->GetSelectedOption(),
		SLD_Reflection->GetValue(), 
		SLD_PostProcessing->GetValue(), 
		SLD_AntiAliasing->GetValue(),
		SLD_GlobalIllumination->GetValue(), 
		SLD_VisualEffect->GetValue(), 
		SLD_ViewDistance->GetValue(),
		SLD_Texture->GetValue(),
		SLD_Shadow->GetValue(),
		SLD_Shading->GetValue(),
		SLD_Foliage->GetValue());
	UpdateUI(GameInstance->GetPlayerGameSettings());
}

void 
UALSGameSettings::BTN_ResetHandler() {
	UALSGameInstance* GameInstance = Cast<UALSGameInstance>(GetGameInstance());
	if (!GameInstance) {
		ALS_WARN(TEXT("Get ALSGameInstance failed: %s:%d"), __FILEW__, __LINE__);
		return;
	}

	GameInstance->ResetGameSettings();
	UpdateUI(GameInstance->GetPlayerGameSettings());
}

void 
UALSGameSettings::DDL_GraphicsQualityHandler(FString SelectedItem, ESelectInfo::Type SelectionType) {
	SCB_GraphicsCustom->SetVisibility(
		SelectedItem == FGraphicsSettings::GraphicsSettingsGraphicsQuality_Custom ? 
		ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void 
UALSGameSettings::SLD_ReflectionHandler(float Value) {
	SLD_InternalHandler(TXT_Reflection, Value);
}

void
UALSGameSettings::SLD_PostProcessingHandler(float Value) {
	SLD_InternalHandler(TXT_PostProcessing, Value);
}

void 
UALSGameSettings::SLD_AntiAliasingHandler(float Value) {
	SLD_InternalHandler(TXT_AntiAliasing, Value);
}

void 
UALSGameSettings::SLD_GlobalIlluminationHandler(float Value) {
	SLD_InternalHandler(TXT_GlobalIllumination, Value);
}

void
UALSGameSettings::SLD_VisualEffectHandler(float Value) {
	SLD_InternalHandler(TXT_VisualEffect, Value);
}

void
UALSGameSettings::SLD_ViewDistanceHandler(float Value) {
	SLD_InternalHandler(TXT_ViewDistance, Value);
}

void 
UALSGameSettings::SLD_TextureHandler(float Value) {
	SLD_InternalHandler(TXT_Texture, Value);
}

void 
UALSGameSettings::SLD_ShadowHandler(float Value) {
	SLD_InternalHandler(TXT_Shadow, Value);
}

void 
UALSGameSettings::SLD_ShadingHandler(float Value) {
	SLD_InternalHandler(TXT_Shading, Value);
}

void 
UALSGameSettings::SLD_FoliageHandler(float Value) {
	SLD_InternalHandler(TXT_Foliage, Value);
}

bool
UALSGameSettings::Initialize() {
	bool bRet = Super::Initialize();

	do {
		UALSGameInstance* GameInstance = Cast<UALSGameInstance>(GetGameInstance());
		if (!GameInstance) {
			ALS_WARN(TEXT("Get ALSGameInstance failed: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!CheckUI()) {
			ALS_FATAL(TEXT("CheckUI failed: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!InitUI(GameInstance)) {
			ALS_FATAL(TEXT("InitUI failed: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		GameInstance->LoadGameSettings();
		UpdateUI(GameInstance->GetPlayerGameSettings());
	} while (0);

	return bRet;
}

inline bool 
UALSGameSettings::CheckUI() const {
	if (!CB_VSync) {
		ALS_ERROR(TEXT("CB_VSync is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!CB_ShowFPS) {
		ALS_ERROR(TEXT("CB_ShowFPS is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!DDL_GraphicsQuality) {
		ALS_ERROR(TEXT("DDL_GraphicsQuality is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!DDL_Resolutions) {
		ALS_ERROR(TEXT("DDL_Resolutions is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!DDL_WindowModes) {
		ALS_ERROR(TEXT("DDL_WindowModes is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!DDL_FrameRateLimits) {
		ALS_ERROR(TEXT("DDL_FrameRateLimits is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SCB_GraphicsCustom) {
		ALS_ERROR(TEXT("SCB_GraphicsCustom is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_Reflection) {
		ALS_ERROR(TEXT("TXT_Reflection is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_Reflection) {
		ALS_ERROR(TEXT("SLD_Reflection is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_PostProcessing) {
		ALS_ERROR(TEXT("TXT_PostProcessing is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_PostProcessing) {
		ALS_ERROR(TEXT("SLD_PostProcessing is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_AntiAliasing) {
		ALS_ERROR(TEXT("TXT_AntiAliasing is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_AntiAliasing) {
		ALS_ERROR(TEXT("SLD_AntiAliasing is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_GlobalIllumination) {
		ALS_ERROR(TEXT("TXT_GlobalIllumination is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_GlobalIllumination) {
		ALS_ERROR(TEXT("SLD_GlobalIllumination is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_VisualEffect) {
		ALS_ERROR(TEXT("TXT_VisualEffect is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_VisualEffect) {
		ALS_ERROR(TEXT("SLD_VisualEffect is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_ViewDistance) {
		ALS_ERROR(TEXT("TXT_ViewDistance is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_ViewDistance) {
		ALS_ERROR(TEXT("SLD_ViewDistance is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_Texture) {
		ALS_ERROR(TEXT("TXT_Texture is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_Texture) {
		ALS_ERROR(TEXT("SLD_Texture is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_Shadow) {
		ALS_ERROR(TEXT("TXT_Shadow is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_Shadow) {
		ALS_ERROR(TEXT("SLD_Shadow is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_Shading) {
		ALS_ERROR(TEXT("TXT_Shading is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_Shading) {
		ALS_ERROR(TEXT("SLD_Shading is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!TXT_Foliage) {
		ALS_ERROR(TEXT("TXT_Foliage is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!SLD_Foliage) {
		ALS_ERROR(TEXT("SLD_Foliage is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!BTN_Confirm) {
		ALS_ERROR(TEXT("BTN_Confirm is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!BTN_Reset) {
		ALS_ERROR(TEXT("BTN_Reset is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	if (!BTN_QuitGame) {
		ALS_ERROR(TEXT("BTN_QuitGame is invalid: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	return true;
}

inline bool 
UALSGameSettings::InitUI(UALSGameInstance* GameInstance) {
	TArray<FIntPoint> Resolutions;
	if (!UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions)) {
		ALS_ERROR(TEXT("UKismetSystemLibrary::GetSupportedFullscreenResolutions() called failed: %s:%d"), __FILEW__, __LINE__);
		return false;
	}

	for (auto& item : Resolutions) {
		DDL_Resolutions->AddOption(FString::Printf(TEXT("%d * %d"), item.X, item.Y));
	}

	DDL_WindowModes->AddOption(FGraphicsSettings::GraphicsSettingsWindowMode_Windowed);
	DDL_WindowModes->AddOption(FGraphicsSettings::GraphicsSettingsWindowMode_FullScreen);
	DDL_WindowModes->AddOption(FGraphicsSettings::GraphicsSettingsWindowMode_WindowedFullScreen);

	DDL_FrameRateLimits->AddOption(FGraphicsSettings::GraphicsSettingsFrameRateLimit_Infinite);
	DDL_FrameRateLimits->AddOption(FGraphicsSettings::GraphicsSettingsFrameRateLimit_60);
	DDL_FrameRateLimits->AddOption(FGraphicsSettings::GraphicsSettingsFrameRateLimit_90);
	DDL_FrameRateLimits->AddOption(FGraphicsSettings::GraphicsSettingsFrameRateLimit_120);

	DDL_GraphicsQuality->AddOption(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Low);
	DDL_GraphicsQuality->AddOption(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Medium);
	DDL_GraphicsQuality->AddOption(FGraphicsSettings::GraphicsSettingsGraphicsQuality_High);
	DDL_GraphicsQuality->AddOption(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Ultra);
	DDL_GraphicsQuality->AddOption(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Extreme);
	DDL_GraphicsQuality->AddOption(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Custom);

	// ------------- 事件绑定 -------------

	FScriptDelegate GraphicsQualityHandler;
	GraphicsQualityHandler.BindUFunction(this, "DDL_GraphicsQualityHandler");
	DDL_GraphicsQuality->OnSelectionChanged.AddUnique(GraphicsQualityHandler);

	FScriptDelegate ReflectionHandler;
	ReflectionHandler.BindUFunction(this, "SLD_ReflectionHandler");
	SLD_Reflection->OnValueChanged.AddUnique(ReflectionHandler);

	FScriptDelegate PostProcessingHandler;
	PostProcessingHandler.BindUFunction(this, "SLD_PostProcessingHandler");
	SLD_PostProcessing->OnValueChanged.AddUnique(PostProcessingHandler);

	FScriptDelegate AntiAliasingHandler;
	AntiAliasingHandler.BindUFunction(this, "SLD_AntiAliasingHandler");
	SLD_AntiAliasing->OnValueChanged.AddUnique(AntiAliasingHandler);

	FScriptDelegate GlobalIlluminationHandler;
	GlobalIlluminationHandler.BindUFunction(this, "SLD_GlobalIlluminationHandler");
	SLD_GlobalIllumination->OnValueChanged.AddUnique(GlobalIlluminationHandler);

	FScriptDelegate VisualEffectHandler;
	VisualEffectHandler.BindUFunction(this, "SLD_VisualEffectHandler");
	SLD_VisualEffect->OnValueChanged.AddUnique(VisualEffectHandler);

	FScriptDelegate ViewDistanceHandler;
	ViewDistanceHandler.BindUFunction(this, "SLD_ViewDistanceHandler");
	SLD_ViewDistance->OnValueChanged.AddUnique(ViewDistanceHandler);

	FScriptDelegate TextureHandler;
	TextureHandler.BindUFunction(this, "SLD_TextureHandler");
	SLD_Texture->OnValueChanged.AddUnique(TextureHandler);

	FScriptDelegate ShadowHandler;
	ShadowHandler.BindUFunction(this, "SLD_ShadowHandler");
	SLD_Shadow->OnValueChanged.AddUnique(ShadowHandler);

	FScriptDelegate ShadingHandler;
	ShadingHandler.BindUFunction(this, "SLD_ShadingHandler");
	SLD_Shading->OnValueChanged.AddUnique(ShadingHandler);

	FScriptDelegate FoliageHandler;
	FoliageHandler.BindUFunction(this, "SLD_FoliageHandler");
	SLD_Foliage->OnValueChanged.AddUnique(FoliageHandler);

	FScriptDelegate ConfirmHandler;
	ConfirmHandler.BindUFunction(this, "BTN_ConfirmHandler");
	BTN_Confirm->OnClicked.AddUnique(ConfirmHandler);

	FScriptDelegate ResetHandler;
	ResetHandler.BindUFunction(this, "BTN_ResetHandler");
	BTN_Reset->OnClicked.AddUnique(ResetHandler);

	FScriptDelegate QuitGameHandler;
	QuitGameHandler.BindUFunction(GameInstance, "QuitGame");
	BTN_QuitGame->OnClicked.AddUnique(QuitGameHandler);

	return true;
}

inline void
UALSGameSettings::UpdateUI(const FGameSettings& GameSettings) {
	const FGraphicsSettings& GraphicsSettings = GameSettings.GraphicsSettings;

	// --------------- 显示设置 ---------------
	CB_VSync->SetIsChecked(GraphicsSettings.VSync);
	CB_ShowFPS->SetIsChecked(GraphicsSettings.ShowFPS);
	DDL_Resolutions->SetSelectedOption(GraphicsSettings.GetResolutionString());
	DDL_WindowModes->SetSelectedOption(GraphicsSettings.GetWindowModeString());
	DDL_FrameRateLimits->SetSelectedOption(GraphicsSettings.GetFrameRateLimitString());
	DDL_GraphicsQuality->SetSelectedOption(GraphicsSettings.GetGraphicsQualityString());
	SCB_GraphicsCustom->SetVisibility(GraphicsSettings.GraphicsQuality == EGraphicsQuality::Custom ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
	SLD_Reflection->SetValue(GraphicsSettings.GetReflectionQuality());
	TXT_Reflection->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.ReflectionQuality)));
	SLD_PostProcessing->SetValue(GraphicsSettings.GetPostProcessingQuality());
	TXT_PostProcessing->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.PostProcessingQuality)));
	SLD_AntiAliasing->SetValue(GraphicsSettings.GetAntiAliasingQuality());
	TXT_AntiAliasing->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.AntiAliasingQuality)));
	SLD_GlobalIllumination->SetValue(GraphicsSettings.GetGlobalIlluminationQuality());
	TXT_GlobalIllumination->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.GlobalIlluminationQuality)));
	SLD_VisualEffect->SetValue(GraphicsSettings.GetVisualEffectQuality());
	TXT_VisualEffect->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.VisualEffectQuality)));
	SLD_ViewDistance->SetValue(GraphicsSettings.GetViewDistanceQuality());
	TXT_ViewDistance->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.ViewDistanceQuality)));
	SLD_Texture->SetValue(GraphicsSettings.GetTextureQuality());
	TXT_Texture->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.TextureQuality)));
	SLD_Shadow->SetValue(GraphicsSettings.GetShadowQuality());
	TXT_Shadow->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.ShadowQuality)));
	SLD_Shading->SetValue(GraphicsSettings.GetShadingQuality());
	TXT_Shading->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.ShadingQuality)));
	SLD_Foliage->SetValue(GraphicsSettings.GetFoliageQuality());
	TXT_Foliage->SetText(FText::FromString(GraphicsSettings.GetGraphicsQualityString(GraphicsSettings.FoliageQuality)));
}

inline void 
UALSGameSettings::SLD_InternalHandler(UTextBlock* TextBlock, float Value) {
	if (Value < 0.7f) {
		TextBlock->SetText(FText::FromString(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Low));
	}
	else if (Value < 1.7f) {
		TextBlock->SetText(FText::FromString(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Medium));
	}
	else if (Value < 2.7f) {
		TextBlock->SetText(FText::FromString(FGraphicsSettings::GraphicsSettingsGraphicsQuality_High));
	}
	else if (Value < 3.7f) {
		TextBlock->SetText(FText::FromString(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Ultra));
	}
	else {
		TextBlock->SetText(FText::FromString(FGraphicsSettings::GraphicsSettingsGraphicsQuality_Extreme));
	}
}