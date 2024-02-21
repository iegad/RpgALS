#include "Core/ALSGameInstance.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSGameInstance)

#include "ALSLibrary.h"
#include "ALSActor.h"

UALSGameInstance::UALSGameInstance() : Super() {
	ALSActorPool = CreateDefaultSubobject<UALSActorPool>(TEXT("ALSActorPool"));
	ALSActorPool->Init(this);
}