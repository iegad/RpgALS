#include "UI/ALSHUD.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSHUD)

#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"

#include "ALSLibrary.h"
#include "Core/ALSGameInstance.h"

void 
UALSHUD::ShowCrosshair() {
	TopCrosshair->SetVisibility(ESlateVisibility::Visible);
	BottomCrosshair->SetVisibility(ESlateVisibility::Visible);
	LeftCrosshair->SetVisibility(ESlateVisibility::Visible);
	RightCrosshair->SetVisibility(ESlateVisibility::Visible);
}

void 
UALSHUD::HideCrosshair() {
	TopCrosshair->SetVisibility(ESlateVisibility::Hidden);
	BottomCrosshair->SetVisibility(ESlateVisibility::Hidden);
	LeftCrosshair->SetVisibility(ESlateVisibility::Hidden);
	RightCrosshair->SetVisibility(ESlateVisibility::Hidden);
}

bool 
UALSHUD::IsCrosshairVisiblity() {
	return TopCrosshair->IsVisible();
}

void 
UALSHUD::CalculateSpread(float Value, float DeltaTime) {
	static const FVector2D InRange(0.f, 650.f);
	static const FVector2D OutRange(5.f, 30.f);

	CrosshairValue = FMath::FInterpTo(CrosshairValue, Value, DeltaTime, 300);
	Spread = FMath::GetMappedRangeValueClamped(InRange, OutRange, CrosshairValue);
}

bool
UALSHUD::Initialize() {
	bool bRet = Super::Initialize();
	HideCrosshair();

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

	return bRet;
}

void
UALSHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	UpdateCrosshairValue(InDeltaTime);
	UpdateShowFPS(InDeltaTime);
}

inline void
UALSHUD::UpdateCrosshairValue(float InDeltaTime) {
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
UALSHUD::UpdateShowFPS(float InDeltaTime) {
	static int32 FPSCollectTimes = 0;
	static float FPSValue = 0.f;
	static float TimerShowFPS = 0.5f;

	UALSGameInstance* GameInstance = Cast<UALSGameInstance>(GetGameInstance());
	if (GameInstance && GameInstance->PlayerGameSettings.ShowFPS) {
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
