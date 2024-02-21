#include "UI/ALSGameSettings.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSGameSettings)

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
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

	FGameSettings& GameSettings = GameInstance->PlayerGameSettings;
	GameSettings.ShowFPS = CB_ShowFPS->IsChecked();
	GameSettings.SetResolution(DDL_Resolutions->GetSelectedOption());
	GameSettings.SetWindowMode(DDL_WindowModes->GetSelectedOption());
	GameSettings.SetFrameRateLimit(DDL_FrameRateLimits->GetSelectedOption());

	UpdateGameSettings(GameSettings);
	UpdateUI(GameSettings);
}

void 
UALSGameSettings::BTN_ResetHandler() {
	UALSGameInstance* GameInstance = Cast<UALSGameInstance>(GetGameInstance());
	if (!GameInstance) {
		ALS_ERROR(TEXT("Get ALSGameInstance failed: %s:%d"), __FILEW__, __LINE__);
		return;
	}

	FGameSettings& GameSettings = GameInstance->PlayerGameSettings;
	GameSettings.Reset();

	UpdateGameSettings(GameSettings);
	UpdateUI(GameSettings);
}

bool
UALSGameSettings::Initialize() {
	bool bRet = Super::Initialize();

	do {
		if (!CB_ShowFPS) {
			ALS_ERROR(TEXT("CB_ShowFPS is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!DDL_Resolutions) {
			ALS_ERROR(TEXT("DDL_Resolutions is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!DDL_WindowModes) {
			ALS_ERROR(TEXT("DDL_WindowModes is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!DDL_FrameRateLimits) {
			ALS_ERROR(TEXT("DDL_FrameRateLimits is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!BTN_Confirm) {
			ALS_ERROR(TEXT("BTN_Confirm is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!BTN_Reset) {
			ALS_ERROR(TEXT("BTN_Reset is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!BTN_QuitGame) {
			ALS_ERROR(TEXT("BTN_QuitGame is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		UALSGameInstance* GameInstance = Cast<UALSGameInstance>(GetGameInstance());
		if (!GameInstance) {
			ALS_ERROR(TEXT("Get ALSGameInstance failed: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		TArray<FIntPoint> Resolutions;
		if (!UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions)) {
			ALS_ERROR(TEXT("UKismetSystemLibrary::GetSupportedFullscreenResolutions() called failed: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		for (auto& item : Resolutions) {
			DDL_Resolutions->AddOption(FString::Printf(TEXT("%d * %d"), item.X, item.Y));
		}

		DDL_WindowModes->AddOption(GameSettingsWindowMode_Windowed);
		DDL_WindowModes->AddOption(GameSettingsWindowMode_FullScreen);
		DDL_WindowModes->AddOption(GameSettingsWindowMode_WindowedFullScreen);

		DDL_FrameRateLimits->AddOption(GameSettingsFrameRateLimit_Infinite);
		DDL_FrameRateLimits->AddOption(GameSettingsFrameRateLimit_60);
		DDL_FrameRateLimits->AddOption(GameSettingsFrameRateLimit_90);
		DDL_FrameRateLimits->AddOption(GameSettingsFrameRateLimit_120);

		FScriptDelegate ConfirmHandler;
		ConfirmHandler.BindUFunction(this, "BTN_ConfirmHandler");
		BTN_Confirm->OnClicked.AddUnique(ConfirmHandler);

		FScriptDelegate ResetHandler;
		ResetHandler.BindUFunction(this, "BTN_ResetHandler");
		BTN_Reset->OnClicked.AddUnique(ResetHandler);

		FScriptDelegate QuitGameHandler;
		QuitGameHandler.BindUFunction(GameInstance, "QuitGame");
		BTN_QuitGame->OnClicked.AddUnique(QuitGameHandler);

		const FGameSettings& GameSettings = GameInstance->PlayerGameSettings;
		UpdateGameSettings(GameSettings);
		UpdateUI(GameSettings);
	} while (0);

	return bRet;
}

inline void 
UALSGameSettings::UpdateUI(const FGameSettings& GameSettings) {
	CB_ShowFPS->SetIsChecked(GameSettings.ShowFPS);
	DDL_Resolutions->SetSelectedOption(GameSettings.GetResolutionString());
	DDL_WindowModes->SetSelectedOption(GameSettings.GetWindowModeString());
	DDL_FrameRateLimits->SetSelectedOption(GameSettings.GetFrameRateLimitString());
}

inline void 
UALSGameSettings::UpdateGameSettings(const FGameSettings& GameSettings) {
	UGameUserSettings* GameUserSettings = GEngine->GetGameUserSettings();
	if (!GameUserSettings) {
		return;
	}

	GameUserSettings->SetScreenResolution(GameSettings.Resolution);
	GameUserSettings->SetFullscreenMode(GameSettings.WindowMode);
	GameUserSettings->SetFrameRateLimit(GameSettings.GetFrameRateLimitValue());
	GameUserSettings->ApplySettings(true);
	GameUserSettings->ApplyResolutionSettings(true);
}
