// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "Containers/Deque.h"

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ALSActor.generated.h"

/**
 * ALSActor, 作为对象池对象, 只能用于C++
 */
UCLASS()
class ALSV4_CPP_API AALSActor : public AActor {
	GENERATED_BODY()
	
public:	
	AALSActor();

	// 设置激活
	void SetActive(bool Value);

	// 设置生存周期
	virtual void SetLifeSpan(float LifeSpan) override;

	// 是否激活
	bool IsActive() const;

	// 获取超时时间
	double GetExpireTime() const;

	// 激活事件: 在激活或失活时触发
	virtual void OnActiveChanged(bool Value) {}

private:
	bool bActived = false;
	double ExpireTime = 0.f;
};

class AALSActor;
class UALSGameInstance;

/**
 * 对象池, 可包含多个不同的对象
 */
UCLASS()
class ALSV4_CPP_API UALSActorPool : public UObject {
	GENERATED_BODY()

	/**
	 * 对象池项
	 */
	struct ALSActorPoolItem {
		TDeque<AALSActor*> UnactivePool;
		TDoubleLinkedList<AALSActor*> ActivePool;

		~ALSActorPoolItem();

		// 获取一个ALSActor对象
		AALSActor* Get(UWorld* World, TSubclassOf<AALSActor> Class, FTransform Transform, float LifeSpan);
		AALSActor* Get(UWorld* World, TSubclassOf<AALSActor> Class, FTransform Transform, float LifeSpan, USceneComponent* Attachment, FName SocketName);

		// 释放一个ALSActor对象到池中
		inline void Put(AALSActor* Actor);

		// 检查生命周期
		void CheckActorLifeSpan();

		// 释放对象池项
		void Release();
	};

public:
	UALSActorPool();

	// 获取一个对象
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AALSActor* Get(UWorld* World, TSubclassOf<AALSActor> Class, FTransform Transform, float LifeSpan);
	AALSActor* Get(UWorld* World, TSubclassOf<AALSActor> Class, FTransform Transform, float LifeSpan, USceneComponent* Attachment, FName SocketName);

	template<typename T>T* Get(UWorld* World, TSubclassOf<AALSActor> Class, FTransform Transform, float LifeSpan) {
		return Cast<T>(Get(World, Class, Transform, LifeSpan));
	}

	template<typename T>T* Get(UWorld* World, TSubclassOf<AALSActor> Class, FTransform Transform, float LifeSpan, USceneComponent* Attachment, FName SocketName) {
		return Cast<T>(Get(World, Class, Transform, LifeSpan, Attachment, SocketName));
	}

	// 将一个对象放中对象池中
	UFUNCTION(BlueprintCallable)
	void Put(AALSActor* Actor);

protected:
	virtual void PostInitProperties() override;
	virtual void BeginDestroy() override;

private:
	void TimerLifeSpanHandler();

	UALSGameInstance* GameInstance;
	FTimerHandle TimerLifeSpan;
	TMap<FString, ALSActorPoolItem*> Pool;
};
