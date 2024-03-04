// Copyright:	Copyright (C) 2024 iegad

#include "Bases/XGameInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XGameInstance)

UXGameInstance::UXGameInstance() : Super() {
	XActorPool = CreateDefaultSubobject<UXActorPoolComponent>(FName{ TEXTVIEW("XActorPool") });
	check(XActorPool);
}
