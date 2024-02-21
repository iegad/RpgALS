#pragma once

#include "Containers/Deque.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALSActor.generated.h"

UCLASS()
class ALSV4_CPP_API AALSActor : public AActor {
	GENERATED_BODY()
	
public:	
	AALSActor();
	void SetActive(bool Value);
	virtual void SetLifeSpan(float LifeSpan) override;
	bool IsActive() const;
	double GetExpireTime() const;
	virtual void OnActiveChanged(bool Value) {}

private:
	bool bActived = false;
	double ExpireTime = 0.f;
};


class AALSActor;
class UALSGameInstance;

struct ALSActorPoolInfo {
	TDeque<AALSActor*> UnactivePool;
	TDoubleLinkedList<AALSActor*> ActivePool;

	~ALSActorPoolInfo();

	AALSActor* Get(UWorld* World, TSubclassOf<AALSActor> Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan);
	void Put(AALSActor* Actor);
	void CheckActorLifeSpan();
	void Release();
};

UCLASS()
class ALSV4_CPP_API UALSActorPool : public UObject {
	GENERATED_BODY()

public:
	UALSActorPool();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AALSActor* Get(UWorld* World, TSubclassOf<AALSActor> Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan);

	UFUNCTION(BlueprintCallable)
	void Put(AALSActor* Actor);

	UFUNCTION(BlueprintCallable)
	void Init(UALSGameInstance* NewGameInstance);

protected:
	virtual void BeginDestroy() override;

private:
	void TimerLifeSpanHandler();

	UALSGameInstance* GameInstance;
	TMap<FString, ALSActorPoolInfo*> Pool;
	FTimerHandle TimerLifeSpan;
};
