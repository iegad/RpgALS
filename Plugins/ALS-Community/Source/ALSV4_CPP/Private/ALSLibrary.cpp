#include "ALSLibrary.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSLibrary)

#include "Core/ALSGameInstance.h"
#include "Core/ALSGameMode.h"

void 
FGameSettings::Reset() {
    ShowFPS = false;
    Resolution = { 1920, 1080 };
    WindowMode = { EWindowMode::Windowed };
    FrameRateLimit = { EFrameRateLimit::Infinite };
}

void 
FGameSettings::SetResolution(const FString& Value) {
    TArray<FString> StringArray;
    if (Value.ParseIntoArray(StringArray, TEXT("*"), true) != 2) {
        ALS_FATAL(TEXT("Value is invalid: %s:%d"), __FILEW__, __LINE__);
    }

    LexFromString(Resolution.X, *StringArray[0]);
    LexFromString(Resolution.Y, *StringArray[1]);
}

void 
FGameSettings::SetWindowMode(const FString& Value) {
    if (Value == GameSettingsWindowMode_Windowed) {
        WindowMode = EWindowMode::Windowed;
    }
    else if (Value == GameSettingsWindowMode_FullScreen) {
        WindowMode = EWindowMode::Fullscreen;
    }
    else if (Value == GameSettingsWindowMode_WindowedFullScreen) {
        WindowMode = EWindowMode::WindowedFullscreen;
    }
    else {
        ALS_FATAL(TEXT("Value is invalid: %s:%d"), __FILEW__, __LINE__);
    }
}

void 
FGameSettings::SetFrameRateLimit(const FString& Value) {
    if (Value == GameSettingsFrameRateLimit_60) {
        FrameRateLimit = EFrameRateLimit::FPS60;
    }
    else if (Value == GameSettingsFrameRateLimit_90) {
        FrameRateLimit = EFrameRateLimit::FPS90;
    }
    else if (Value == GameSettingsFrameRateLimit_120) {
        FrameRateLimit = EFrameRateLimit::FPS120;
    }
    else if (Value == GameSettingsFrameRateLimit_Infinite) {
        FrameRateLimit = EFrameRateLimit::Infinite;
    }
    else {
        ALS_FATAL(TEXT("Value is invalid: %s:%d"), __FILEW__, __LINE__);
    }
}

float 
FGameSettings::GetFrameRateLimitValue() const {
    switch (FrameRateLimit) {
    case EFrameRateLimit::FPS60: return 60.f;
    case EFrameRateLimit::FPS90: return 90.f;
    case EFrameRateLimit::FPS120: return 120.f;
    }

    return 0.f;
}

FString 
FGameSettings::GetResolutionString() const {
    return FString::Printf(TEXT("%d * %d"), Resolution.X, Resolution.Y);
}

FString 
FGameSettings::GetWindowModeString() const {
    switch (WindowMode) {
    case EWindowMode::Fullscreen: return GameSettingsWindowMode_FullScreen;
    case EWindowMode::WindowedFullscreen: return GameSettingsWindowMode_WindowedFullScreen;
    }

    return GameSettingsWindowMode_Windowed;
}

FString 
FGameSettings::GetFrameRateLimitString() const {
    switch (FrameRateLimit) {
    case EFrameRateLimit::FPS60: return GameSettingsFrameRateLimit_60;
    case EFrameRateLimit::FPS90: return GameSettingsFrameRateLimit_90;
    case EFrameRateLimit::FPS120: return GameSettingsFrameRateLimit_120;
    }

    return GameSettingsFrameRateLimit_Infinite;
}

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