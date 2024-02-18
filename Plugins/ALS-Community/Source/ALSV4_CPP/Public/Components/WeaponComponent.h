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
