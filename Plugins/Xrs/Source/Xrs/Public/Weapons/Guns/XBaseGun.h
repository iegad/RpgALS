// Copyright:	Copyright (C) 2024 iegad

#pragma once

#include "Weapons/Guns/XGunData.h"

#include "CoreMinimal.h"
#include "Bases/XActor.h"
#include "XBaseGun.generated.h"

class UBoxComponent;
class UWidgetComponent;

/**
 * 
 */
UCLASS()
class XRS_API AXBaseGun : public AXActor {
	GENERATED_BODY()
	
public:
	AXBaseGun();

	UFUNCTION(BlueprintCallable)
	bool Lock();

	UFUNCTION(BlueprintCallable)
	void UnLock();

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FXGunData& GetData() const { return *Data; }

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|DataTable")
	FName RowID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Xrs|DataTable")
	TObjectPtr<UDataTable> XGunDataTable;

	void DisableCollision();
	void EnableCollision();

protected:

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Xrs|Components")
	TObjectPtr<UBoxComponent> BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Xrs|Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

private:
	UFUNCTION()
	void OnActorBeginOverlapHandler(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnActorEndOverlapHandler(AActor* OverlappedActor, AActor* OtherActor);

	inline bool LoadData();

	bool bLock = false;
	FXGunData* Data = nullptr;
};
