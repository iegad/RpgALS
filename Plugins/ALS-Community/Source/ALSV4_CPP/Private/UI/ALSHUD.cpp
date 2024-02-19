#include "UI/ALSHUD.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSHUD)

#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Blueprint/WidgetTree.h"

void 
UALSHUD::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	do {
		if (!IsCrosshairVisiblity()) {
			break;
		}

		if (!TopCrosshair || !BottomCrosshair || !LeftCrosshair || !RightCrosshair) {
			break;
		}

		if (!TopCrosshairSlot) {
			TopCrosshairSlot = Cast<UCanvasPanelSlot>(TopCrosshair->Slot);
			if (!TopCrosshairSlot) {
				break;
			}
		}

		if (!BottomCrosshairSlot) {
			BottomCrosshairSlot = Cast<UCanvasPanelSlot>(BottomCrosshair->Slot);
			if (!BottomCrosshairSlot) {
				break;
			}
		}

		if (!LeftCrosshairSlot) {
			LeftCrosshairSlot = Cast<UCanvasPanelSlot>(LeftCrosshair->Slot);
			if (!LeftCrosshairSlot) {
				break;
			}
		}

		if (!RightCrosshairSlot) {
			RightCrosshairSlot = Cast<UCanvasPanelSlot>(RightCrosshair->Slot);
			if (!RightCrosshairSlot) {
				break;
			}
		}

		BottomCrosshairSlot->SetSize(FVector2D(Thickness, Length));
		BottomCrosshairSlot->SetPosition(FVector2D(0 - Thickness / 2, Spread));

		TopCrosshairSlot->SetSize(FVector2D(Thickness, Length));
		TopCrosshairSlot->SetPosition(FVector2D(0 - Thickness / 2, 0 - Length - Spread));

		LeftCrosshairSlot->SetSize(FVector2D(Length, Thickness));
		LeftCrosshairSlot->SetPosition(FVector2D(0 - Length - Spread, 0 - Thickness / 2));

		RightCrosshairSlot->SetSize(FVector2D(Length, Thickness));
		RightCrosshairSlot->SetPosition(FVector2D(Spread, 0 - Thickness / 2));
	} while (0);
}

bool 
UALSHUD::Initialize() {
	bool bRet = Super::Initialize();
	HideCrosshair();

	return bRet;
}

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
	return TopCrosshair->GetVisibility() == ESlateVisibility::Visible &&
		BottomCrosshair->GetVisibility() == ESlateVisibility::Visible &&
		LeftCrosshair->GetVisibility() == ESlateVisibility::Visible &&
		RightCrosshair->GetVisibility() == ESlateVisibility::Visible;
}

void 
UALSHUD::CalculateSpread(float Value, float DeltaTime) {
	static const FVector2D InRange(0.f, 650.f);
	static const FVector2D OutRange(5.f, 30.f);

	CrosshairValue = FMath::FInterpTo(CrosshairValue, Value, DeltaTime, 36);
	Spread = FMath::GetMappedRangeValueClamped(InRange, OutRange, CrosshairValue);
}
