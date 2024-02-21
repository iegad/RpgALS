#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ALSHUD.generated.h"

class UBorder;
class UCanvasPanelSlot;
class UTextBlock;

UCLASS()
class ALSV4_CPP_API UALSHUD : public UUserWidget {
	GENERATED_BODY()
	
public:
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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RightTopFPS;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> RightTopFPSLable;

protected:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	inline void UpdateCrosshairValue(float InDeltaTime);
	inline void UpdateShowFPS(float InDeltaTime);

	// -------------------------------- 瞄准镜相关字段 --------------------------------
	UCanvasPanelSlot* TopCrosshairSlot = nullptr;
	UCanvasPanelSlot* BottomCrosshairSlot = nullptr;
	UCanvasPanelSlot* LeftCrosshairSlot = nullptr;
	UCanvasPanelSlot* RightCrosshairSlot = nullptr;

	float CrosshairValue = 0.f;
	float Spread = 5.f;
	float Length = 10.f;
	float Thickness = 4.f;
};
