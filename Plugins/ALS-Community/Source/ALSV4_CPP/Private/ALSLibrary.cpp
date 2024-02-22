// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "ALSLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSLibrary)

#include "Core/ALSGameInstance.h"
#include "Core/ALSGameMode.h"

const FString FGraphicsSettings::GraphicsSettingsGraphicsQuality_Custom{ TEXT("Custom") };
const FString FGraphicsSettings::GraphicsSettingsGraphicsQuality_Low{ TEXT("Low") };
const FString FGraphicsSettings::GraphicsSettingsGraphicsQuality_Medium{ TEXT("Medium") };
const FString FGraphicsSettings::GraphicsSettingsGraphicsQuality_High{ TEXT("High") };
const FString FGraphicsSettings::GraphicsSettingsGraphicsQuality_Ultra{ TEXT("Ultra") };
const FString FGraphicsSettings::GraphicsSettingsGraphicsQuality_Extreme{ TEXT("Extreme") };


const FString FGraphicsSettings::GraphicsSettingsWindowMode_Windowed{ TEXT("Windowed") };
const FString FGraphicsSettings::GraphicsSettingsWindowMode_FullScreen{ TEXT("Full Screen") };
const FString FGraphicsSettings::GraphicsSettingsWindowMode_WindowedFullScreen{ TEXT("Windowed Full Screen") };

const FString FGraphicsSettings::GraphicsSettingsFrameRateLimit_60{ TEXT("FPS 60") };
const FString FGraphicsSettings::GraphicsSettingsFrameRateLimit_90{ TEXT("FPS 90") };
const FString FGraphicsSettings::GraphicsSettingsFrameRateLimit_120{ TEXT("FPS 120") };
const FString FGraphicsSettings::GraphicsSettingsFrameRateLimit_Infinite{ TEXT("Infinite") };

const FGameSettings FGameSettings::DefaultGameSettings{};

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