// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "Components/PropsComponent.h"
#include "WeaponComponent.generated.h"

class AWeaponBase;

UCLASS()
class ALSV4_CPP_API UWeaponComponent : public UPropsComponent {
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void Attack(AWeaponBase* WeaponBase, int DebugTrace = 2);
};
