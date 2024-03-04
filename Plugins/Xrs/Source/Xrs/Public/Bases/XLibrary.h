// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include <fstream>

#include "Bases/XGameInstance.h"
#include "Components/XActorPoolComponent.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "XLibrary.generated.h"

/**
 * 日志宏
 */
#define __XTEXT_TOKEN_(token1, token2) TEXT(token1 token2)
#define __XLOG_(Category, Format, ...) UE_LOG(LogTemp, Category, __XTEXT_TOKEN_("[%s:%d] ", Format), __FILEW__, __LINE__, ##__VA_ARGS__)
#define XINFO(Format, ...) __XLOG_(Display, Format, ##__VA_ARGS__)
#define XWARN(Format, ...) __XLOG_(Warning, Format, ##__VA_ARGS__)
#define XERROR(Format, ...) __XLOG_(Error, Format, ##__VA_ARGS__)
#define XFATAL(Format, ...) __XLOG_(Fatal, Format, ##__VA_ARGS__)

#define XASSERT(expr, Format, ...) if (!(expr)) XFATAL(Format, ##__VA_ARGS__)

/**
 * 
 */
UCLASS()
class XRS_API UXLibrary : public UBlueprintFunctionLibrary {
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Xrs|Core")
	static UXGameInstance* GetGameInstance(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Xrs|Core")
	static APlayerController* GetPlayerController(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Xrs|Core")
	static APlayerCameraManager* GetPlayerCameraManager(UObject* WorldContextObject);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Xrs|XActor")
	static AXActor* GetActor(UObject* WorldContextObject, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan);

	template<typename T> 
	static T* GetActor(UWorld* World, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan, USceneComponent* Attachment = nullptr, const FAttachmentTransformRules& AttachRules = FAttachmentTransformRules::SnapToTargetIncludingScale, const FName& SocketName = NAME_None);
};

template<typename T> T* 
UXLibrary::GetActor(UWorld* World, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan, USceneComponent* Attachment, const FAttachmentTransformRules& AttachRules, const FName& SocketName) {
	UXGameInstance* GameInstance = GetGameInstance();
	XASSERT(GameInstance, "GetGameInstance failed");
	return GameInstance->GetActorPool()->Get<T>(World, Class, TransformCast, LifeSpan, Attachment, AttachRules, SocketName);
}
