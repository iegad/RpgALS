#pragma once

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
