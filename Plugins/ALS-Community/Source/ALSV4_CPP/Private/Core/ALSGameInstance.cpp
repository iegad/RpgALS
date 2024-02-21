// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Core/ALSGameInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSGameInstance)

#include "Kismet/KismetSystemLibrary.h"

#include "ALSLibrary.h"
#include "ALSActor.h"

void
UALSGameInstance::QuitGame() {
	RequestEngineExit(TEXT("Exit"));
}

UALSGameInstance::UALSGameInstance() : Super() {
	ALSActorPool = CreateDefaultSubobject<UALSActorPool>(TEXT("ALSActorPool"));
	ALSActorPool->Init(this);
}