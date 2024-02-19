#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ALSHUD.generated.h"

class UBorder;
class UCanvasPanelSlot;

UCLASS()
class ALSV4_CPP_API UALSHUD : public UUserWidget {
	GENERATED_BODY()
	
public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual bool Initialize() override;

	void ShowCrosshair();
	void HideCrosshair();
	bool IsCrosshairVisiblity();
	void CalculateSpread(float Value, float DeltaTime);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> TopCrosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BottomCrosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> LeftCrosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> RightCrosshair;

private:
	// -------------------------------- 瞄准镜相关字段 --------------------------------
	UCanvasPanelSlot* TopCrosshairSlot = nullptr;
	UCanvasPanelSlot* BottomCrosshairSlot = nullptr;
	UCanvasPanelSlot* LeftCrosshairSlot = nullptr;
	UCanvasPanelSlot* RightCrosshairSlot = nullptr;

	float CrosshairValue = 0.f;
	float Spread = 5.f;
	float Length = 6.f;
	float Thickness = 2.5f;
};
