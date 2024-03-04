// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EXGunType {
	EGT_Invalid,
	EGT_Rifle,
	EFT_Pistol,
};

UENUM(BlueprintType)
enum class EXBulletType {
	Rifle,
	Pistol,
	Sniper,
};