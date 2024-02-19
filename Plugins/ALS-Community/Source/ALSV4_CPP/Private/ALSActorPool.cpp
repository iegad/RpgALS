#include "ALSActorPool.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSActorPool)

#include "ALSActor.h"

UALSActorPool::UALSActorPool() : Super() {
}

UALSActorPool::~UALSActorPool() {
	while (!UnactivePool.empty()) {
		AActor* Actor = UnactivePool.top();
		UnactivePool.pop();
		Actor->Destroy();
	}

	while (!ActivePool.empty()) {
		auto itr = ActivePool.begin();
		AActor* Actor = *itr;
		Actor->Destroy();
		ActivePool.erase(itr);
	}
}

AALSActor* 
UALSActorPool::Get(UWorld* World, UClass* Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan) {
	AALSActor* Actor = nullptr;

	do {
		if (!World || !Class || LifeSpan < 0.f) {
			break;
		}

		if (UnactivePool.empty()) {
			AActor* actor = World->SpawnActor(Class, &Location, &Rotation);
			if (!IsValid(actor)) {
				break;
			}

			Actor = Cast<AALSActor>(actor);
			if (!IsValid(Actor)) {
				actor->Destroy();
				break;
			}
		}
		else {
			Actor = UnactivePool.top();
			UnactivePool.pop();
			if (!Actor) {
				break;
			}

			FTransform Transform;
			Transform.SetLocation(Location);
			Transform.SetRotation(FQuat(Rotation));

			Actor->SetActorTransform(Transform);
		}

		Actor->SetActive(true);
		Actor->SetLifeSpan(LifeSpan);
		ActivePool.emplace_back(Actor);

		FTimerManager& TimerManager = World->GetTimerManager();
		if (!TimerManager.IsTimerActive(LifeSpanTimer)) {
			TimerManager.SetTimer(LifeSpanTimer, this, &UALSActorPool::CheckActorLifeSpan, 1.f, true);
		}
	} while (0);

	return Actor;
}

void 
UALSActorPool::Put(AALSActor* Actor) {
	if (Actor) {
		Actor->SetActive(false);
		UnactivePool.push(Actor);
	}
}

void 
UALSActorPool::CheckActorLifeSpan() {
	double TimeNow = 0.;

	for (auto itr = ActivePool.begin(); itr != ActivePool.end();) {
		AALSActor* Actor = *itr;
		if (!Actor) {
			ActivePool.erase(itr++);
			continue;
		}

		if (!Actor->IsActive()) {
			ActivePool.erase(itr++);
			Put(Actor);
			continue;
		}

		TimeNow = Actor->GetWorld()->TimeSeconds;

		if (Actor->GetExpireTime() && TimeNow >= Actor->GetExpireTime()) {
			ActivePool.erase(itr++);
			Put(Actor);
			continue;
		}

		itr++;
	}
}
