#include "Props/Weapons/Guns/Magazine.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(Magazine)

AMagazine::AMagazine() : Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void 
AMagazine::OnActiveChanged(bool Value) {
	if (Value) {
		Mesh->SetWorldLocationAndRotation(GetActorLocation(), GetActorRotation());
	}
	else {
		Mesh->SetSimulatePhysics(false);
		Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
