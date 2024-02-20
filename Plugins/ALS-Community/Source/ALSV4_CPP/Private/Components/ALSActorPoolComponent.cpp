#include "Components/ALSActorPoolComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSActorPoolComponent)

#include "ALSActor.h"
#include "ALSLibrary.h"

UALSActorPoolComponent::UALSActorPoolComponent() : Super() {
	// PrimaryComponentTick.bCanEverTick = true;
}

AALSActor* 
UALSActorPoolComponent::Get(TSubclassOf<AALSActor> Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan) {
	AALSActor* Actor = nullptr;

	UWorld* World = GetWorld();

	do {
		if (!World || !Class || LifeSpan < 0.f) {
			break;
		}

		if (!UnactivePool.IsEmpty() && UnactivePool.TryPopFirst(Actor) && Actor) {
			Actor->SetActorTransform(FTransform(Rotation, Location));
		}
		else {
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

		Actor->SetActive(true);
		Actor->SetLifeSpan(LifeSpan);
		ActivePool.AddTail(Actor);

	} while (0);

	return Actor;
}

AALSActor* 
UALSActorPoolComponent::Get(const FVector& Location, const FRotator& Rotation, const float LifeSpan) {
	return ALSActorClass ? Get(ALSActorClass, Location, Rotation, LifeSpan) : nullptr;
}

void 
UALSActorPoolComponent::Put(AALSActor* Actor) {
	if (Actor) {
		Actor->SetActive(false);
		UnactivePool.PushLast(Actor);
	}
}

void 
UALSActorPoolComponent::BeginDestroy() {
	ALS_WARN(TEXT("--------- %s BeginDestroy ---------"), *this->GetName());

	UWorld* World = GetWorld();
	if (World) {
		World->GetTimerManager().ClearTimer(LifeSpanTimer);
	}

	while (!UnactivePool.IsEmpty()) {
		AALSActor* Actor = nullptr;
		if (UnactivePool.TryPopFirst(Actor) && Actor) {
			Actor->Destroy();
		}
	}
	UnactivePool.Empty();

	while (!ActivePool.IsEmpty()) {
		auto* Node = ActivePool.GetHead();
		if (Node) {
			AALSActor* Actor = Node->GetValue();
			if (Actor) {
				Actor->Destroy();
			}
			ActivePool.RemoveNode(Node);
		}
	}
	ActivePool.Empty();

	Super::BeginDestroy();
}

void 
UALSActorPoolComponent::OnComponentCreated() {
	Super::OnComponentCreated();

	UWorld* World = GetWorld();
	if (!World) {
		ALS_ERROR(TEXT("%s GetWorld() failed: %s:%d"), *this->GetName(), __FILEW__, __LINE__);
		return;
	}

	FTimerManager& TimerManager = World->GetTimerManager();
	if (!TimerManager.IsTimerActive(LifeSpanTimer)) {
		TimerManager.SetTimer(LifeSpanTimer, this, &UALSActorPoolComponent::CheckActorLifeSpan, 1.f, true);
	}
}

void 
UALSActorPoolComponent::CheckActorLifeSpan() {
	double TimeNow = 0.;

	auto* Node = ActivePool.GetHead();
	while (Node) {
		AALSActor* Actor = Node->GetValue();
		if (!Actor) {
			auto* DeleteNode = Node;
			Node = Node->GetNextNode();
			ActivePool.RemoveNode(DeleteNode);
			continue;
		}

		if (!Actor->IsActive()) {
			auto* DeleteNode = Node;
			Node = Node->GetNextNode();
			ActivePool.RemoveNode(DeleteNode);
			UnactivePool.PushLast(Actor);
			continue;
		}

		TimeNow = Actor->GetWorld()->TimeSeconds;

		if (Actor->GetExpireTime() && TimeNow >= Actor->GetExpireTime()) {
			auto* DeleteNode = Node;
			Node = Node->GetNextNode();
			ActivePool.RemoveNode(DeleteNode);
			UnactivePool.PushLast(Actor);
			Actor->SetActive(false);
			continue;
		}

		Node = Node->GetNextNode();
	}
}
