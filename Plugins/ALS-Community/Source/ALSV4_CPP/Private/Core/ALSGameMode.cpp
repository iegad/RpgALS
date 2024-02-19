#include "Core/ALSGameMode.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSGameMode)

#include "Sound/SoundBase.h"
#include "NiagaraSystem.h"
#include "Kismet/GameplayStatics.h"

#include "ALSLibrary.h"
#include "ALSActor.h"
#include "Core/ALSPlayerController.h"

AALSGameMode::AALSGameMode() : Super() {
}

void 
AALSGameMode::PlayEffect(EPhysicalSurface Surface, const FVector& Location, const FVector& Normal, UParticleSystem* AdditiveVFX) {
	UWorld* World = GetWorld();
	if (!World) {
		return;
	}

	const EEnvironmentPhysicalMaterials PhysicalMaterial = EEnvironmentPhysicalMaterials(Surface);

	UParticleSystem* Particle = EnvironmentVFX.FindRef(PhysicalMaterial);
	USoundBase* Sound = EnvironmentSFX.FindRef(PhysicalMaterial);

	const FRotator&& Rotation = Normal.ToOrientationRotator();

	if (Sound) {
		UGameplayStatics::SpawnSoundAtLocation(World, Sound, Location, Rotation);
	}
	
	if (Particle) {
		UGameplayStatics::SpawnEmitterAtLocation(World, Particle, Location, Rotation);
	}

	if (AdditiveVFX) {
		UGameplayStatics::SpawnEmitterAtLocation(World, AdditiveVFX, Location, Rotation);
	}
}

AALSActor* 
AALSGameMode::GetALSActor(UClass* Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan) {
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

void 
AALSGameMode::PutALSActor(AALSActor* Actor) {
	if (Actor) {
		Actor->SetActive(false);
		UnactivePool.PushLast(Actor);
	}
}

void 
AALSGameMode::BeginPlay() {
	if (!EnvironmentSFX.Num()) {
		ALS_ERROR(TEXT("%s's EnvironmentSFX is not set: %s:%d"), *this->GetClass()->GetName(), __FILEW__, __LINE__);
	}

	if (!EnvironmentVFX.Num()) {
		ALS_ERROR(TEXT("%s's EnvironmentVFX is not set: %s:%d"), *this->GetClass()->GetName(), __FILEW__, __LINE__);
	}

	FTimerManager& TimerManager = GetWorld()->GetTimerManager();
	if (!TimerManager.IsTimerActive(LifeSpanTimer)) {
		TimerManager.SetTimer(LifeSpanTimer, this, &AALSGameMode::CheckActorLifeSpan, 1.f, true);
	}
}

void 
AALSGameMode::BeginDestroy() {
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
AALSGameMode::CheckActorLifeSpan() {
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
			continue;
		}

		Node = Node->GetNextNode();
	}
}
