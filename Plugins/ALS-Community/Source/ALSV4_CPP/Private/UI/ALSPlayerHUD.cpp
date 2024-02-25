// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "UI/ALSPlayerHUD.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSPlayerHUD)

#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"

#include "ALSLibrary.h"
#include "Core/ALSGameInstance.h"
#include "Props/Weapons/Guns/GunBase.h"

void 
UALSPlayerHUD::ShowFPS() {
	if (HBOX_FPS->GetVisibility() == ESlateVisibility::Hidden) {
		HBOX_FPS->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void 
UALSPlayerHUD::HideFPS() {
	if (HBOX_FPS->GetVisibility() != ESlateVisibility::Hidden) {
		HBOX_FPS->SetVisibility(ESlateVisibility::Hidden);
	}
}

void
UALSPlayerHUD::ShowCrosshair() {
	if (CP_Crosshair->GetVisibility() == ESlateVisibility::Hidden) {
		CP_Crosshair->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void 
UALSPlayerHUD::HideCrosshair() {
	if (CP_Crosshair->GetVisibility() != ESlateVisibility::Hidden) {
		CP_Crosshair->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool
UALSPlayerHUD::IsCrosshairVisiblity() const {
	return CP_Crosshair->GetVisibility() != ESlateVisibility::Hidden;
}

void 
UALSPlayerHUD::SetGun(AGunBase* Gun) {
	if (HBOX_RifleAmmo->GetVisibility() == ESlateVisibility::Hidden) {
		HBOX_RifleAmmo->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	TXT_CurrentRifleAmmo->SetText(FText::AsNumber(Gun->AmmoInfo.CurrentAmmo));
	TXT_MaxRifleAmmo->SetText(FText::AsNumber(Gun->AmmoInfo.MaxAmmo));
}

void 
UALSPlayerHUD::ResetGun() {
	if (HBOX_RifleAmmo->GetVisibility() != ESlateVisibility::Hidden) {
		HBOX_RifleAmmo->SetVisibility(ESlateVisibility::Hidden);
	}
}

void
UALSPlayerHUD::CalculateSpread(float Value, float DeltaTime) {
	static const FVector2D InRange(0.f, 650.f);
	static const FVector2D OutRange(5.f, 30.f);

	CrosshairValue = FMath::FInterpTo(CrosshairValue, Value, DeltaTime, 300);
	Spread = FMath::GetMappedRangeValueClamped(InRange, OutRange, CrosshairValue);
}

void
UALSPlayerHUD::SetMaxAmmo(int32 Value) {
	if (Value >= 0) {
		TXT_MaxRifleAmmo->SetText(FText::AsNumber(Value));
	}
}

void
UALSPlayerHUD::SetCurrentAmmo(int32 Value) {
	if (Value >= 0) {
		TXT_CurrentRifleAmmo->SetText(FText::AsNumber(Value));
	}
}

bool
UALSPlayerHUD::Initialize() {
	bool bRet = Super::Initialize();

	if (!HBOX_FPS) {
		ALS_ERROR(TEXT("HBOX_FPS is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TXT_FPS) {
		ALS_ERROR(TEXT("TXT_FPS is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!CP_Crosshair) {
		ALS_ERROR(TEXT("CP_Crosshair is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TopCrosshair) {
		ALS_ERROR(TEXT("TopCrosshair is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!BottomCrosshair) {
		ALS_ERROR(TEXT("BottomCrosshair is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!LeftCrosshair) {
		ALS_ERROR(TEXT("LeftCrosshair is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!RightCrosshair) {
		ALS_ERROR(TEXT("RightCrosshair is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!HBOX_RifleAmmo) {
		ALS_ERROR(TEXT("HBOX_RifleAmmo is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TXT_CurrentRifleAmmo) {
		ALS_ERROR(TEXT("TXT_CurrentRifleAmmo is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TXT_MaxRifleAmmo) {
		ALS_ERROR(TEXT("TXT_MaxRifleAmmo is not exists: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TopCrosshairSlot) {
		TopCrosshairSlot = Cast<UCanvasPanelSlot>(TopCrosshair->Slot);
	}

	if (!BottomCrosshairSlot) {
		BottomCrosshairSlot = Cast<UCanvasPanelSlot>(BottomCrosshair->Slot);
	}

	if (!LeftCrosshairSlot) {
		LeftCrosshairSlot = Cast<UCanvasPanelSlot>(LeftCrosshair->Slot);
	}

	if (!RightCrosshairSlot) {
		RightCrosshairSlot = Cast<UCanvasPanelSlot>(RightCrosshair->Slot);
	}

	if (!TopCrosshairSlot) {
		ALS_FATAL(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!BottomCrosshairSlot) {
		ALS_FATAL(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!LeftCrosshairSlot) {
		ALS_FATAL(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!RightCrosshairSlot) {
		ALS_FATAL(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TXT_CurrentRifleAmmo) {
		ALS_ERROR(TEXT("TXT_CurrentAmmo builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TXT_MaxRifleAmmo) {
		ALS_ERROR(TEXT("TXT_MaxAmmo builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	HideFPS();
	HideCrosshair();
	return bRet;
}

void
UALSPlayerHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCrosshairValue(InDeltaTime);
	UpdateShowFPS(InDeltaTime);
}

inline void
UALSPlayerHUD::UpdateCrosshairValue(float InDeltaTime) {
	if (IsCrosshairVisiblity()) {
		BottomCrosshairSlot->SetSize(FVector2D(Thickness, Length));
		BottomCrosshairSlot->SetPosition(FVector2D(0 - Thickness / 2, Spread));

		TopCrosshairSlot->SetSize(FVector2D(Thickness, Length));
		TopCrosshairSlot->SetPosition(FVector2D(0 - Thickness / 2, 0 - Length - Spread));

		LeftCrosshairSlot->SetSize(FVector2D(Length, Thickness));
		LeftCrosshairSlot->SetPosition(FVector2D(0 - Length - Spread, 0 - Thickness / 2));

		RightCrosshairSlot->SetSize(FVector2D(Length, Thickness));
		RightCrosshairSlot->SetPosition(FVector2D(Spread, 0 - Thickness / 2));
	}
}

inline void 
UALSPlayerHUD::UpdateShowFPS(float InDeltaTime) {
	static int32 FPSCollectTimes = 0;
	static float FPSValue = 0.f;
	static float TimerShowFPS = 0.5f;

	UALSGameInstance* GameInstance = Cast<UALSGameInstance>(GetGameInstance());

	if (GameInstance && GameInstance->GetPlayerGameSettings().GraphicsSettings.ShowFPS) {
		ShowFPS();
		
		if (TimerShowFPS <= 0.f) {
			TXT_FPS->SetText(FText::AsNumber(int32(FPSValue / (float)FPSCollectTimes)));
			TimerShowFPS = 0.5f;
			FPSCollectTimes = 0;
			FPSValue = 0.f;
		}
		else {
			FPSValue += 1 / InDeltaTime;
			TimerShowFPS -= InDeltaTime;
			FPSCollectTimes++;
		}
	}
	else {
		HideFPS();
	}
}
