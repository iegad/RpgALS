// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include <stack>

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "XActorPoolComponent.generated.h"

class AXActor;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XRS_API UXActorPoolComponent : public UActorComponent {
	GENERATED_BODY()

	struct Item {
		std::stack<AXActor*> UnactiveStack;
		std::list<AXActor*> ActiveList;

		~Item();

		AXActor* Get(UWorld* World, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan, USceneComponent* Attachment, const FAttachmentTransformRules& AttachRules, const FName& SocketName);

		void Put(AXActor* XActor);

		void CheckLifeSpan(double TimeNow);
	};

public:	
	/// <summary>
	/// 从对象池中获取XActor实例
	/// </summary>
	/// <param name="Class"></param>
	/// <param name="Transform"></param>
	/// <param name="LifeSpan">生存周期, 如果 LifeSpan 为 0, 表示无限制生命周期, 需要手动调用Put来释放对象到对象池</param>
	/// <returns></returns>
	UFUNCTION(BlueprintCallable, BlueprintPure)
	AXActor* Get(TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan);

	/// <summary>
	/// 将XActor释放到对象池中
	/// </summary>
	/// <param name="XActor"></param>
	UFUNCTION(BlueprintCallable)
	void Put(AXActor* XActor);

	template<class T>
	T* Get(UWorld* World, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan, USceneComponent* Attachment = nullptr, const FAttachmentTransformRules& AttachRules = FAttachmentTransformRules::SnapToTargetIncludingScale, const FName& SocketName = NAME_None) {
		return Cast<T>(Get(World, Class, Transform, LifeSpan, Attachment, AttachRules, SocketName));
	}

	AXActor* Get(UWorld* World, TSubclassOf<AXActor> Class, const FTransform& Transform, float LifeSpan, USceneComponent* Attachment = nullptr, const FAttachmentTransformRules& AttachRules = FAttachmentTransformRules::SnapToTargetIncludingScale, const FName& SocketName = NAME_None);
	

protected:
	virtual void BeginPlay() override;
	virtual void BeginDestroy() override;

private:
	void TimerLifeSpanHandler();

	UGameInstance* GameInstance = nullptr;
	FTimerHandle TimerLifeSpan;
	TMap<FName, Item*> Pool;
};
