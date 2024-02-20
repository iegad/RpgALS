#include "Props/Weapons/MarkerBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(MarkerBase)

#include "ALSLibrary.h"
#include "Components/DecalComponent.h"

AMarkerBase::AMarkerBase() : Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
	Decal->SetupAttachment(Root);
	Decal->SetFadeScreenSize(0.f);
}