#pragma once

#include "Containers/Deque.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ALSActorPoolComponent.generated.h"

class AALSActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UALSActorPoolComponent : public UActorComponent {
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AALSActor> ALSActorClass;

public:	
	UALSActorPoolComponent();

	AALSActor* Get(TSubclassOf<AALSActor> Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan);
	AALSActor* Get(const FVector& Location, const FRotator& Rotation, const float LifeSpan);
	void Put(AALSActor* Actor);

protected:
	virtual void BeginDestroy() override;
	virtual void OnComponentCreated() override;

private:
	void CheckActorLifeSpan();

private:
	TDeque<AALSActor*> UnactivePool;
	TDoubleLinkedList<AALSActor*> ActivePool;
	FTimerHandle LifeSpanTimer;
};
