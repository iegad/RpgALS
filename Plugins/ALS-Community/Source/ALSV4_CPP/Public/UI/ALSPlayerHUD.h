// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ALSPlayerHUD.generated.h"

class UBorder;
class UCanvasPanelSlot;
class UTextBlock;

UCLASS()
class ALSV4_CPP_API UALSPlayerHUD : public UUserWidget {
	GENERATED_BODY()
	
public:
	void ShowCrosshair();
	void HideCrosshair();
	void ShowAmmoInfo(int32 Value, int32 MaxValue);
	void HideAmmoInfo();
	bool IsCrosshairVisiblity() const;
	void CalculateSpread(float Value, float DeltaTime);
	void SetMaxAmmo(int32 Value);
	void SetCurrentAmmo(int32 Value);

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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_CurrentAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_MaxAmmo;

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
