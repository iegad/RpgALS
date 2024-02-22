// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "ALSLibrary.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ALSGameSettings.generated.h"

class UButton;
class UCheckBox;
class UComboBoxString;
class UScrollBox;
class USlider;
class UTextBlock;

UCLASS()
class ALSV4_CPP_API UALSGameSettings : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BTN_ConfirmHandler();

	UFUNCTION(BlueprintCallable)
	void BTN_ResetHandler();

	UFUNCTION(BlueprintCallable)
	void DDL_GraphicsQualityHandler(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION(BlueprintCallable)
	void SLD_ReflectionHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_PostProcessingHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_AntiAliasingHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_GlobalIlluminationHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_VisualEffectHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_ViewDistanceHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_TextureHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_ShadowHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_ShadingHandler(float Value);

	UFUNCTION(BlueprintCallable)
	void SLD_FoliageHandler(float Value);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> CB_VSync;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> CB_ShowFPS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> DDL_Resolutions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> DDL_WindowModes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> DDL_FrameRateLimits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> DDL_GraphicsQuality;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UScrollBox> SCB_GraphicsCustom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_Reflection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_Reflection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_PostProcessing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_PostProcessing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_AntiAliasing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_AntiAliasing;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_GlobalIllumination;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_GlobalIllumination;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_VisualEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_VisualEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_ViewDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_ViewDistance;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_Texture;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_Shadow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_Shadow;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_Shading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_Shading;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UTextBlock> TXT_Foliage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USlider> SLD_Foliage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BTN_Confirm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BTN_Reset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BTN_QuitGame;

	virtual bool Initialize() override;

private:
	inline bool CheckUI() const;
	inline bool InitUI(UALSGameInstance* GameInstance);
	inline void UpdateUI(const FGameSettings& GameSettings);
	inline void SLD_InternalHandler(UTextBlock* TextBlock, float Value);
};
