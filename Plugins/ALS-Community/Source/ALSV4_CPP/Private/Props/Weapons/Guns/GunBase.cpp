#include "Props/Weapons/Guns/GunBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(GunBase)

#include "ALSLibrary.h"

AGunBase::AGunBase() : Super() {
	MuzzleScene = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleScene"));
	MuzzleScene->SetupAttachment(GetRootComponent());
}

void
AGunBase::BeginPlay() {
	Super::BeginPlay();

	if (MuzzleSocketName.IsNone()) {
		ALS_ERROR(TEXT("MuzzleSocketName is invalid: %s:%d"), __FILEW__, __LINE__);
	}
}
