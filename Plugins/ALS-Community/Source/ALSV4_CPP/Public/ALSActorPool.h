#pragma once

#include <stack>

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ALSActorPool.generated.h"

class AALSActor;

UCLASS()
class ALSV4_CPP_API UALSActorPool : public UObject {
	GENERATED_BODY()
	
public:
	UALSActorPool();

	virtual ~UALSActorPool();

	AALSActor* Get(UWorld* World, UClass* Class, const FVector& Location, const FRotator& Rotation, const float LifeSpan);

	void Put(AALSActor* Actor);

private:
	void CheckActorLifeSpan();

	std::stack<AALSActor*> UnactivePool;
	std::list<AALSActor*> ActivePool;
	FTimerHandle LifeSpanTimer;
};
