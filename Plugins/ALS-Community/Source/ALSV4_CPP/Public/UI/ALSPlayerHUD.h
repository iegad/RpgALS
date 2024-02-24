// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ALSPlayerHUD.generated.h"

class UBorder;
class UCanvasPanel;
class UCanvasPanelSlot;
class UHorizontalBox;
class UTextBlock;

UCLASS()
class ALSV4_CPP_API UALSPlayerHUD : public UUserWidget {
	GENERATED_BODY()
	
public:
	void ShowFPS();
	void HideFPS();

	void ShowCrosshair();
	void HideCrosshair();
	bool IsCrosshairVisiblity() const;
	void ShowRifleAmmo(int32 Value, int32 MaxValue);
	void HideRifleAmmo();
	
	void CalculateSpread(float Value, float DeltaTime);
	void SetRifleMaxAmmo(int32 Value);
	void SetRifleCurrentAmmo(int32 Value);

protected:

	// FPS 相关UI
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HBOX_FPS;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_FPS;

	// 准心
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCanvasPanel> CP_Crosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> TopCrosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> BottomCrosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> LeftCrosshair;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> RightCrosshair;

	// 弹药
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UHorizontalBox> HBOX_RifleAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_CurrentRifleAmmo;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_MaxRifleAmmo;

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
