#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ALSGameInstance.generated.h"

class UALSActorPool;

UCLASS()
class ALSV4_CPP_API UALSGameInstance : public UGameInstance {
	GENERATED_BODY()
	
public:
	UALSGameInstance();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ALS|ALSActor Pool")
	TObjectPtr<UALSActorPool> ALSActorPool;
};
