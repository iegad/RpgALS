// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "UI/ALSPlayerHUD.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSPlayerHUD)

#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"

#include "ALSLibrary.h"
#include "Core/ALSGameInstance.h"

void 
UALSPlayerHUD::ShowCrosshair() {
	TopCrosshair->SetVisibility(ESlateVisibility::Visible);
	BottomCrosshair->SetVisibility(ESlateVisibility::Visible);
	LeftCrosshair->SetVisibility(ESlateVisibility::Visible);
	RightCrosshair->SetVisibility(ESlateVisibility::Visible);
}

void 
UALSPlayerHUD::HideCrosshair() {
	TopCrosshair->SetVisibility(ESlateVisibility::Hidden);
	BottomCrosshair->SetVisibility(ESlateVisibility::Hidden);
	LeftCrosshair->SetVisibility(ESlateVisibility::Hidden);
	RightCrosshair->SetVisibility(ESlateVisibility::Hidden);
}

void 
UALSPlayerHUD::ShowAmmoInfo(int32 Value, int32 MaxValue) {
	TXT_CurrentAmmo->SetText(FText::AsNumber(Value));
	TXT_MaxAmmo->SetText(FText::AsNumber(MaxValue));
	TXT_CurrentAmmo->SetVisibility(ESlateVisibility::Visible);
	TXT_MaxAmmo->SetVisibility(ESlateVisibility::Visible);
}

void 
UALSPlayerHUD::HideAmmoInfo() {
	TXT_CurrentAmmo->SetVisibility(ESlateVisibility::Hidden);
	TXT_MaxAmmo->SetVisibility(ESlateVisibility::Hidden);
}

bool 
UALSPlayerHUD::IsCrosshairVisiblity() const {
	return TopCrosshair->IsVisible();
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
		TXT_MaxAmmo->SetText(FText::AsNumber(Value));
	}
}

void 
UALSPlayerHUD::SetCurrentAmmo(int32 Value) {
	if (Value >= 0) {
		TXT_CurrentAmmo->SetText(FText::AsNumber(Value));
	}
}

bool
UALSPlayerHUD::Initialize() {
	bool bRet = Super::Initialize();

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
		ALS_ERROR(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!BottomCrosshairSlot) {
		ALS_ERROR(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!LeftCrosshairSlot) {
		ALS_ERROR(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!RightCrosshairSlot) {
		ALS_ERROR(TEXT("TopCrosshairSlot builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TXT_CurrentAmmo) {
		ALS_ERROR(TEXT("TXT_CurrentAmmo builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	if (!TXT_MaxAmmo) {
		ALS_ERROR(TEXT("TXT_MaxAmmo builded failed: %s:%d"), __FILEW__, __LINE__);
	}

	HideCrosshair();
	HideAmmoInfo();
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
	if (IsCrosshairVisiblity() && TopCrosshair && BottomCrosshair && LeftCrosshair && RightCrosshair &&
		TopCrosshairSlot && BottomCrosshairSlot && LeftCrosshairSlot && RightCrosshairSlot) {
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
		if (!RightTopFPS->IsVisible()) {
			RightTopFPS->SetVisibility(ESlateVisibility::Visible);
			RightTopFPSLable->SetVisibility(ESlateVisibility::Visible);
		}

		if (TimerShowFPS <= 0.f) {
			RightTopFPS->SetText(FText::AsNumber((int32)(FPSValue / (float)FPSCollectTimes)));
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
	else if (RightTopFPS->IsVisible()) {
		RightTopFPS->SetVisibility(ESlateVisibility::Hidden);
		RightTopFPSLable->SetVisibility(ESlateVisibility::Hidden);
	}
}
