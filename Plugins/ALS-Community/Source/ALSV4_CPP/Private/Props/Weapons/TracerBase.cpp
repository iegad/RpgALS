#include "Props/Weapons/TracerBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TracerBase)

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "ALSLibrary.h"

ATracerBase::ATracerBase() : Super() {
	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);

	Tracer = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Tracer"));
	Tracer->SetupAttachment(Box);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed = Speed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void 
ATracerBase::BeginPlay() {
	Super::BeginPlay();

	if (Speed <= 0.f) {
		ALS_ERROR(TEXT("%s Speed is invalid: %s:%d"), *this->GetName(), __FILEW__, __LINE__);
	}
}

void
ATracerBase::OnActiveChanged(bool Value) {
	if (Value) {
		ProjectileMovement->Velocity = GetActorRotation().Vector() * Speed;
		ProjectileMovement->UpdateComponentVelocity();
		Tracer->Activate(true);
	}
	else {
		ProjectileMovement->StopMovementImmediately();
		Tracer->Activate(false);
	}
}
