// Copyright:	Copyright (C) 2024 iegad

#include "Bases/XLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XLibrary)

#include "Kismet/GameplayStatics.h"

UXGameInstance* 
UXLibrary::GetGameInstance(UObject* WorldContextObject) {
	UGameInstance* GameInstance = UGameplayStatics::GetGameInstance(WorldContextObject);

	if (!GameInstance) {
		XERROR("UGameplayStatics::GetGameInstance called failed");
		return nullptr;
	}

	UXGameInstance* XGameInstance = Cast<UXGameInstance>(GameInstance);
	if (XGameInstance) {
		XERROR("GameInstance is not a XGameInstance");
		return nullptr;
	}

	return XGameInstance;
}

APlayerController* 
UXLibrary::GetPlayerController(UObject* WorldContextObject) {
	return UGameplayStatics::GetPlayerController(WorldContextObject, 0);
}

APlayerCameraManager* 
UXLibrary::GetPlayerCameraManager(UObject* WorldContextObject) {
	return UGameplayStatics::GetPlayerCameraManager(WorldContextObject, 0);
}

AXActor* 
UXLibrary::GetActor(UObject* WorldContextObject, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan) {
	UXGameInstance* GameInstance = GetGameInstance(WorldContextObject);
	XASSERT(GameInstance, "GetGameInstance() called failed");
	return GameInstance->GetActorPool()->Get(Class, Transform, LifeSpan);
}