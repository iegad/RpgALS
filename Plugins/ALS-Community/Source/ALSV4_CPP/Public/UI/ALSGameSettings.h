#pragma once

#include "ALSLibrary.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ALSGameSettings.generated.h"

class UCheckBox;
class UButton;
class UComboBoxString;

UCLASS()
class ALSV4_CPP_API UALSGameSettings : public UUserWidget {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void BTN_ConfirmHandler();

	UFUNCTION(BlueprintCallable)
	void BTN_ResetHandler();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> CB_ShowFPS;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> DDL_Resolutions;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> DDL_WindowModes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> DDL_FrameRateLimits;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BTN_Confirm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BTN_Reset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UButton> BTN_QuitGame;

	virtual bool Initialize() override;

private:
	inline void UpdateUI(const FGameSettings& GameSettings);
	inline void UpdateGameSettings(const FGameSettings& GameSettings);
};
