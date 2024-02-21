#include "ALSLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSLibrary)

#include "Core/ALSGameInstance.h"
#include "Core/ALSGameMode.h"

UWorld* 
UALSLibrary::GetCurrentWorld() const {
    FWorldContext* World = IsRunningDedicatedServer() ? GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport) : nullptr;
    return World ? World->World() : nullptr;
}

UALSGameInstance* 
UALSLibrary::GetGameInstance(UWorld* World) const {
    if (!World) {
        World = GetWorld();
    }

    return World ? Cast<UALSGameInstance>(World->GetGameInstance()) : nullptr;
}

AALSGameMode* 
UALSLibrary::GetGameMode(UWorld* World) const {
    if (!World) {
        World = GetWorld();
    }

    return World ? Cast<AALSGameMode>(World->GetAuthGameMode()) : nullptr;
}
