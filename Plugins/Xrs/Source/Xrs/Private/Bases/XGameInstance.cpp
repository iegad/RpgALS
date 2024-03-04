// Copyright:	Copyright (C) 2024 iegad

#include "Bases/XGameInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XGameInstance)

#include "Bases/XLibrary.h"

UXGameInstance::UXGameInstance() : Super() {
	XActorPool = CreateDefaultSubobject<UXActorPoolComponent>(FName{ TEXTVIEW("XActorPool") });
	XASSERT(XActorPool, "CreateDefaultSubobject UXActorPoolComponent failed");
}
