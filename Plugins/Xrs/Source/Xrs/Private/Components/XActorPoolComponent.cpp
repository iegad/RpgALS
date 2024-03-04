// Copyright:	Copyright (C) 2024 iegad

#include "Components/XActorPoolComponent.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XActorPoolComponent)

#include "Bases/XActor.h"
#include "Bases/XLibrary.h"
#include "Kismet/GameplayStatics.h"

UXActorPoolComponent::Item::~Item() {
	while (!ActiveList.empty()) {
		auto itr = ActiveList.begin();
		AXActor* Actor = *itr;
		if (Actor) {
			Actor->Destroy();
		}
		ActiveList.erase(itr);
	}

	while (!UnactiveStack.empty()) {
		AXActor* Actor = UnactiveStack.top();
		if (Actor) {
			Actor->Destroy();
		}
		UnactiveStack.pop();
	}
}

AXActor*
UXActorPoolComponent::Item::Get(UWorld* World, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan, USceneComponent* Attachment, const FAttachmentTransformRules& AttachRules, const FName& SocketName) {
	AXActor* Ret = nullptr;

	do {
		if (UnactiveStack.empty()) {
			AActor* Actor = World->SpawnActor(Class, &Transform);
			XASSERT(Actor, "World->SpawnActor called failed");

			Ret = Cast<AXActor>(Actor);
			XASSERT(Ret, "%s is not a XActor", *Class->GetName());
		}
		else {
			Ret = UnactiveStack.top();
			XASSERT(Ret, "UnactiveStack has nullptr Item");
			Ret->SetActorTransform(Transform);
			Ret->SetLifeSpan(LifeSpan);
		}

		if (Attachment) {
			Ret->AttachToComponent(Attachment, AttachRules, SocketName);
		}

		Ret->OnCreated(Transform, Attachment, SocketName);
		Ret->SetActive(true);
	} while (0);

	return Ret;
}

void 
UXActorPoolComponent::Item::Put(AXActor* XActor) {
	if (XActor) {
		XActor->OnDestroy();
		XActor->SetActive(false);
		UnactiveStack.push(XActor);
	}
}

void 
UXActorPoolComponent::Item::CheckLifeSpan(double TimeNow) {
	for (auto itr = ActiveList.begin(); itr != ActiveList.end();) {
		AXActor* Actor = *itr;
		if (!Actor) {
			ActiveList.erase(itr++);
			continue;
		}

		if (!Actor->IsActive()) {
			ActiveList.erase(itr++);
			Put(Actor);
			continue;
		}

		if (Actor->GetExpireTime() > 0 && Actor->GetExpireTime() >= TimeNow) {
			ActiveList.erase(itr++);
			Put(Actor);
			continue;
		}

		itr++;
	}
}

AXActor* 
UXActorPoolComponent::Get(TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan) {
	return Get(GetWorld(), Class, Transform, LifeSpan);
}

void 
UXActorPoolComponent::Put(AXActor* XActor) {
	if (XActor) {
		Item* itr = Pool.FindRef(XActor->StaticClass()->GetFName());
		XASSERT(itr, "There is no Class[%s] in pool", *XActor->StaticClass()->GetFName().ToString());
		itr->Put(XActor);
	}
}

AXActor*
UXActorPoolComponent::Get(UWorld* World, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan, USceneComponent* Attachment, const FAttachmentTransformRules& AttachRules, const FName& SocketName) {
	do {
		if (!World) {
			World = GetWorld();
		}

		if (!World) {
			XFATAL("World is invalid");
		}

		if (!Class) {
			XFATAL("Class is invalid");
		}

		if (LifeSpan < 0.) {
			XFATAL("LifeSpan is invalid");
		}

		const FName&& ClassName = Class->GetFName();
		Item* itr = Pool.FindRef(ClassName);
		if (!itr) {
			itr = Pool.Add(ClassName, new Item);
			XASSERT(itr, "Pool Add Item failed");
		}

		XASSERT(itr, "itr is invalid");
		return itr->Get(World, Class, Transform, LifeSpan, Attachment, AttachRules, SocketName);
	} while (0);
	return nullptr;
}

void 
UXActorPoolComponent::BeginPlay() {
	Super::BeginPlay();

	GameInstance = Cast<UGameInstance>(GetOuter());
	if (GameInstance) {
		if (GameInstance->IsA<UXGameInstance>()) {
			XFATAL("You can't make this XActorPool when the GameInstance is a UXGameInstance");
		}
		return;
	}

	GameInstance = UGameplayStatics::GetGameInstance(GetWorld());
	if (!GameInstance) {
		XWARN("XActorPoolComponent won't work. Because get GameInstance failed.");
		return;
	}

	Rename(TEXT("XActorPoolComponent"), GameInstance);
	XINFO("%s's outer has been changed to: %s", *this->GetName(), *GetOuter()->GetName());
	
	FTimerManager& TimerManager = GameInstance->GetTimerManager();
	XASSERT(!TimerManager.IsTimerActive(TimerLifeSpan), "TimerLifeSpan is Actived");
	TimerManager.SetTimer(TimerLifeSpan, this, &UXActorPoolComponent::TimerLifeSpanHandler, 1.f, true, 1.f);
}

void 
UXActorPoolComponent::BeginDestroy() {
	XINFO("%s BeginDestroy .", *GetName());

	if (GameInstance) {
		FTimerManager& TimerManager = GameInstance->GetTimerManager();
		XASSERT(TimerManager.IsTimerActive(TimerLifeSpan), "TimerLifeSpan is not active");
		TimerManager.ClearTimer(TimerLifeSpan);

		for (auto& itr : Pool) {
			delete itr.Value;
		}
		Pool.Empty();
	}

	Super::BeginDestroy();
}

void 
UXActorPoolComponent::TimerLifeSpanHandler() {
	UWorld* World = GetWorld();
	if (!World) {
		return;
	}

	double TimeNow = World->TimeSeconds;

	for (auto& itr : Pool) {
		itr.Value->CheckLifeSpan(TimeNow);
	}
}
