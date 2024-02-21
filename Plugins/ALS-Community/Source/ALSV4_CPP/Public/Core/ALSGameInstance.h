#pragma once

#include "ALSLibrary.h"

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ALSGameInstance.generated.h"

class UALSActorPool;

UCLASS()
class ALSV4_CPP_API UALSGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|ALSActor Pool")
	TObjectPtr<UALSActorPool> ALSActorPool;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Game Settings")
	FGameSettings DefaultGameSettings;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|Game Settings")
	FGameSettings PlayerGameSettings;

	UFUNCTION(BlueprintCallable, Category = "ALS|Game Settings")
	void QuitGame();
	
public:
	UALSGameInstance();
};
