#pragma once

#include "CoreMinimal.h"
#include "ALSActor.h"
#include "TracerBase.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;

UCLASS()
class ALSV4_CPP_API ATracerBase : public AALSActor {
	GENERATED_BODY()

public:
	ATracerBase();

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> Box;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UParticleSystemComponent> Tracer;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Speed = 0.f;
	
protected:
	virtual void BeginPlay() override;
	virtual void OnActiveChanged(bool Value) override;
};
