#include "Components/WeaponComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WeaponComponent)

#include "ALSLibrary.h"
#include "Character/ALSCharacter.h"
#include "Props/Weapons/WeaponBase.h"

void 
UWeaponComponent::Attack(AWeaponBase* WeaponBase, int DebugTrace) {
	AALSCharacter* Character = Cast<AALSCharacter>(GetOuter());

	if (!Character) {
		ALS_ERROR(TEXT("%s's Outer is not ASLCharacter: %s:%d"), *this->GetName(), __FILEW__, __LINE__);
		return;
	}

	WeaponBase->Attack(Character, DebugTrace);
}
