#pragma once

#include "CoreMinimal.h"
#include "ALSActor.h"
#include "Magazine.generated.h"

UCLASS()
class ALSV4_CPP_API AMagazine : public AALSActor {
	GENERATED_BODY()

public:
	AMagazine();

protected:
	virtual void OnActiveChanged(bool Value) override;
	
public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> Mesh;
};
