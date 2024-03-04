// Copyright:	Copyright (C) 2024 iegad

#include "Bases/XActor.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XActor)

#include "Bases/XLibrary.h"

AXActor::AXActor() : Super() {
}

void 
AXActor::OnCreated_Implementation(const FTransform& Transform, USceneComponent* Attachment, const FName& BoneName){
}

void
AXActor::OnDestroy_Implementation() {
}

void
AXActor::SetLifeSpan(float LifeSpan) {
	UWorld* World = GetWorld();
	ExpireTime = World && LifeSpan > 0. ? World->TimeSeconds + LifeSpan : 0.;
}