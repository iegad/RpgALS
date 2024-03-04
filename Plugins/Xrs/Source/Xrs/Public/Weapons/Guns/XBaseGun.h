// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include "CoreMinimal.h"
#include "Bases/XActor.h"
#include "XBaseGun.generated.h"

class UBoxComponent;
class UXGunDataAsset;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class XRS_API AXBaseGun : public AXActor {
	GENERATED_BODY()
	
public:
	AXBaseGun();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|DataAssets")
	TObjectPtr<UXGunDataAsset> DataAsset;

	UFUNCTION(BlueprintCallable)
	bool Lock();

	UFUNCTION(BlueprintCallable)
	void UnLock();

	void DisableCollision();
	void EnableCollision();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Xrs|Components")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Xrs|Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
	UFUNCTION()
	void OnActorBeginOverlapHandler(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnActorEndOverlapHandler(AActor* OverlappedActor, AActor* OtherActor);

	void LoadData();

	bool bLock = false;
};
