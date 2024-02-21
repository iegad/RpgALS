#include "ALSActor.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSActor)

#include "ALSActor.h"
#include "ALSLibrary.h"
#include "Core/ALSGameInstance.h"

AALSActor::AALSActor() : Super() {
	// PrimaryComponentTick.bCanEverTick = true;
}

void 
AALSActor::SetActive(bool Value) {
	bActived = Value;
	SetActorHiddenInGame(!Value);
	OnActiveChanged(Value);
}

void 
AALSActor::SetLifeSpan(float LifeSpan) {
	ExpireTime = LifeSpan ? GetWorld()->TimeSeconds + LifeSpan : 0;
}

bool 
AALSActor::IsActive() const {
	return bActived;
}

double 
AALSActor::GetExpireTime() const {
	return ExpireTime;
}


ALSActorPoolInfo::~ALSActorPoolInfo() {
	Release();
}

AALSActor*
ALSActorPoolInfo::Get(UWorld* World, TSubclassOf<AALSActor> Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan) {
	AALSActor* Actor = nullptr;

	do {
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

void
ALSActorPoolInfo::Put(AALSActor* Actor) {
	if (Actor) {
		Actor->SetActive(false);
		UnactivePool.PushLast(Actor);
	}
}

void
ALSActorPoolInfo::CheckActorLifeSpan() {
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

void
ALSActorPoolInfo::Release() {
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
}

UALSActorPool::UALSActorPool() : Super() {
	// PrimaryComponentTick.bCanEverTick = true;
}

AALSActor*
UALSActorPool::Get(UWorld* World, TSubclassOf<AALSActor> Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan) {
	AALSActor* Actor = nullptr;

	do {
		if (!World) {
			ALS_ERROR(TEXT("World is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!Class) {
			ALS_ERROR(TEXT("Class is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!LifeSpan) {
			ALS_ERROR(TEXT("LifeSpan[%f] is invalid: %s:%d"), LifeSpan, __FILEW__, __LINE__);
			break;
		}

		FString ClassName = Class->GetName();
		if (!Pool.Contains(ClassName)) {
			Pool.Add(ClassName, new ALSActorPoolInfo);
		}

		ALSActorPoolInfo* PoolInfo = Pool.FindRef(ClassName);
		if (!PoolInfo) {
			ALS_ERROR(TEXT("ClassName[%s] is not exists: %s:%d"), *ClassName, __FILEW__, __LINE__);
			break;
		}

		Actor = PoolInfo->Get(World, Class, Location, Rotation, LifeSpan);
	} while (0);

	return Actor;
}

void
UALSActorPool::Put(AALSActor* Actor) {
	do {
		if (!Actor) {
			break;
		}

		FString ClassName = Actor->GetClass()->GetName();
		if (!Pool.Contains(ClassName)) {

		}
	} while (0);
}

void
UALSActorPool::Init(UALSGameInstance* NewGameInstance) {
	do {
		if (!GameInstance || GameInstance != NewGameInstance) {
			GameInstance = NewGameInstance;
		}

		if (!GameInstance) {
			ALS_ERROR(TEXT("GameInstance is invalid: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		if (!GameInstance) {
			ALS_ERROR(TEXT("Get ALSGameInstance failed. Please check GameInstance setting: %s:%d"), __FILEW__, __LINE__);
			break;
		}

		FTimerManager& TimerManager = GameInstance->GetTimerManager();
		if (!TimerManager.IsTimerActive(TimerLifeSpan)) {
			TimerManager.SetTimer(TimerLifeSpan, this, &UALSActorPool::TimerLifeSpanHandler, 1.f, true);
		}
	} while (0);
}

void
UALSActorPool::BeginDestroy() {
	ALS_WARN(TEXT("--------------- BeginDestroy ---------------: %s:%d"), __FILEW__, __LINE__);

	if (GameInstance) {
		FTimerManager& TimerManager = GameInstance->GetTimerManager();
		if (!TimerManager.IsTimerActive(TimerLifeSpan)) {
			TimerManager.SetTimer(TimerLifeSpan, this, &UALSActorPool::TimerLifeSpanHandler, 1.f, true);
		}
	}

	for (auto& item : Pool) {
		if (item.Value) {
			delete item.Value;
		}
	}
	Pool.Empty();

	Super::BeginDestroy();
}

void
UALSActorPool::TimerLifeSpanHandler() {
	for (auto& item : Pool) {
		if (item.Value) {
			item.Value->CheckActorLifeSpan();
		}
	}
}