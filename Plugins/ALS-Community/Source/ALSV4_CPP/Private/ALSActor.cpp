#include "ALSActor.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSActor)

AALSActor::AALSActor() : Super() {
}

void 
AALSActor::SetActive(bool Value) {
	bActived = Value;
	SetActorHiddenInGame(!Value);
	OnActiveChanged(Value);
}

void 
AALSActor::SetLifeSpan(float LifeSpan) {
	ExpireTime = LifeSpan ? GetWorld()->TimeSeconds + LifeSpan : 0;
}

bool 
AALSActor::IsActive() const {
	return bActived;
}

double 
AALSActor::GetExpireTime() const {
	return ExpireTime;
}
