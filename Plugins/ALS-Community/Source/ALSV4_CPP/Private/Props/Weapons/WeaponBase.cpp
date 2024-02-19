#include "Props/Weapons/WeaponBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(WeaponBase)

#include "ALSLibrary.h"

bool 
FWeaponAttackOptions::CheckInitialized() const {
    bool bRet = true;

    if (AttackInterval <= 0.f) {
        ALS_ERROR(TEXT("AttackInterval is invalid: %s:%d"), __FILEW__, __LINE__);
        if (bRet) bRet = false;
    }

    if (Range <= 0.f) {
        ALS_ERROR(TEXT("Range is invalid: %s:%d"), __FILEW__, __LINE__);
        if (bRet) bRet = false;
    }

    if (Damage <= 0.f) {
        ALS_ERROR(TEXT("Damage is invalid: %s:%d"), __FILEW__, __LINE__);
        if (bRet) bRet = false;
    }

    return bRet;
}

bool
FWeaponEffectsOptions::CheckInitialized() const {
    bool bRet = true;

    if (!AttackSound) {
        ALS_ERROR(TEXT("AttackSound is invalid: %s:%d"), __FILEW__, __LINE__);
        if (bRet) bRet = false;
    }

    if (!AttackEffects) {
        ALS_ERROR(TEXT("AttackEffects is invalid: %s:%d"), __FILEW__, __LINE__);
        if (bRet) bRet = false;
    }

    if (!HittedEffects) {
        ALS_ERROR(TEXT("HittedEffects is invalid: %s:%d"), __FILEW__, __LINE__);
        if (bRet) bRet = false;
    }

    return bRet;
}

bool 
FWeaponAnimationOptions::CheckInitialized() const {
    bool bRet = true;

    if (!AttackMontage) {
        ALS_ERROR(TEXT("AttackMontage is invalid: %s:%d"), __FILEW__, __LINE__);
        if (bRet) bRet = false;
    }

    return bRet;
}



AWeaponBase::AWeaponBase() : Super() {
    PrimaryActorTick.bCanEverTick = true;
}

void 
AWeaponBase::BeginPlay() {
    Super::BeginPlay();

    WeaponAttackOptions.CheckInitialized();
    WeaponEffectsOptions.CheckInitialized();
    WeaponAnimationOptions.CheckInitialized();
}

void 
AWeaponBase::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    if (WeaponAttackOptions.AttackCD > 0) {
        WeaponAttackOptions.AttackCD -= DeltaTime;
    }
}
