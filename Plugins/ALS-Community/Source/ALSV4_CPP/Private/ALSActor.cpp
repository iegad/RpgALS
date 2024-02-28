// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

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
	ExpireTime = LifeSpan > 0.f ? GetWorld()->TimeSeconds + LifeSpan : 0.;
}

bool 
AALSActor::IsActive() const {
	return bActived;
}

double 
AALSActor::GetExpireTime() const {
	return ExpireTime;
}


UALSActorPool::ALSActorPoolItem::~ALSActorPoolItem() {
	Release();
}

AALSActor*
UALSActorPool::ALSActorPoolItem::Get(UWorld* World, TSubclassOf<AALSActor> Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan) {
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

AALSActor* 
UALSActorPool::ALSActorPoolItem::Get(UWorld* World, TSubclassOf<AALSActor> Class, const float LifeSpan, USceneComponent* Attachment, FName SocketName) {
	AALSActor* Actor = nullptr;

	do {
		if (!Attachment || SocketName.IsNone()) {
			break;
		}
		auto tmp = Attachment->GetSocketTransform(SocketName);
		const FVector&& Location = tmp.GetLocation();
		const FRotator && Rotation = tmp.GetRotation().Rotator();

		if (!UnactivePool.IsEmpty() && UnactivePool.TryPopFirst(Actor) && Actor) {
			Actor->AttachToComponent(Attachment, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);
			Actor->SetActorTransform(FTransform(Rotation, Location));
		}
		else {
			AActor* actor = World->SpawnActor(Class, &Location, &Rotation);
			if (!IsValid(actor)) {
				break;
			}

			actor->AttachToComponent(Attachment, FAttachmentTransformRules::SnapToTargetIncludingScale, SocketName);

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

inline void
UALSActorPool::ALSActorPoolItem::Put(AALSActor* Actor) {
	if (Actor) {
		Actor->SetActive(false);
		UnactivePool.PushLast(Actor);
	}
}

void
UALSActorPool::ALSActorPoolItem::CheckActorLifeSpan() {
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
UALSActorPool::ALSActorPoolItem::Release() {
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
UALSActorPool::Get(UWorld* World, TSubclassOf<AALSActor> Class, const float LifeSpan, USceneComponent* Attachment, FName SocketName) {
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

		FString ClassName = Class->GetName();
		if (!Pool.Contains(ClassName)) {
			Pool.Add(ClassName, new ALSActorPoolItem);
		}

		ALSActorPoolItem* PoolInfo = Pool.FindRef(ClassName);
		if (!PoolInfo) {
			ALS_ERROR(TEXT("ClassName[%s] is not exists: %s:%d"), *ClassName, __FILEW__, __LINE__);
			break;
		}

		Actor = PoolInfo->Get(World, Class, LifeSpan, Attachment, SocketName);
	} while (0);

	return Actor;
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

		FString ClassName = Class->GetName();
		if (!Pool.Contains(ClassName)) {
			Pool.Add(ClassName, new ALSActorPoolItem);
		}

		ALSActorPoolItem* PoolInfo = Pool.FindRef(ClassName);
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
			Actor->Destroy();
			break;
		}

		ALSActorPoolItem* PoolInfo = Pool.FindRef(ClassName);
		PoolInfo->Put(Actor);
	} while (0);
}

void
UALSActorPool::PostInitProperties() {
	Super::PostInitProperties();

	do {
		GameInstance = Cast<UALSGameInstance>(GetOuter());

		if (!GameInstance) {
			ALS_WARN(TEXT("UALSActorPool must mount ALSGameInstance: %s:%d"), __FILEW__, __LINE__);
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