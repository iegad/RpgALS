// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "XGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class XRS_API UXGameInstance : public UGameInstance {
	GENERATED_BODY()

public:
	UXGameInstance();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Xrs|ActorPool")
	FORCEINLINE class UXActorPoolComponent* GetActorPool() const { return XActorPool; }
	
protected:
	TObjectPtr<class UXActorPoolComponent> XActorPool;
};
