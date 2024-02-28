// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Props/Weapons/Guns/GunBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GunBase)

#include "KismetTraceUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "ALSLibrary.h"
#include "Character/ALSCharacter.h"
#include "Core/ALSGameInstance.h"
#include "Core/ALSGameMode.h"
#include "Props/Weapons/TracerBase.h"
#include "Props/Weapons/MarkerBase.h"

AGunBase::AGunBase() : Super() {
	MuzzleScene = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleScene"));
	MuzzleScene->SetupAttachment(GetRootComponent());
}