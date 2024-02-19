#include "ALSLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSLibrary)

UWorld* 
UALSLibrary::GetCurrentWorld() const {
    FWorldContext* World = IsRunningDedicatedServer() ? GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport) : nullptr;
    return World ? World->World() : nullptr;
}
