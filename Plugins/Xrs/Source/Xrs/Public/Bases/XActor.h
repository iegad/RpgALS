// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "XActor.generated.h"

UCLASS()
class XRS_API AXActor : public AActor {
	GENERATED_BODY()
	
public:
	AXActor();

	// 设置激活状态
	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetActive(bool bActived) { SetActorHiddenInGame(!bActived); }

	// 获取激活状态
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsActive() const { return IsHidden(); }

	// 获取过期时间
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE double GetExpireTime() const { return ExpireTime; }

	// 创建事件
	UFUNCTION(BlueprintNativeEvent, Category = "Xrs|Events")
	void OnCreated(const FTransform &Transform, USceneComponent* Attachment, const FName &BoneName);

	// 销毁事件
	UFUNCTION(BlueprintNativeEvent, Category = "Xrs|Events")
	void OnDestroy();

	// 获取 XGameInstance 实例
	inline class UXGameInstance* GetGameInstance() const { return AActor::GetGameInstance<UXGameInstance>(); }
protected:
	// 设置生存周期
	virtual void SetLifeSpan(float LifeSpan) override;

private:
	double ExpireTime = 0.;

	friend class UXActorPoolComponent;
};
