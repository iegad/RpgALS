// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ALSLibrary.generated.h"

/**
 * 日志宏
 */
#define ALS_LOG(Format, ...) UE_LOG(LogTemp, Display, Format, ##__VA_ARGS__)
#define ALS_WARN(Format, ...) UE_LOG(LogTemp, Warning, Format, ##__VA_ARGS__)
#define ALS_ERROR(Format, ...) UE_LOG(LogTemp, Error, Format, ##__VA_ARGS__)
#define ALS_FATAL(Format, ...) UE_LOG(LogTemp, Fatal, Format, ##__VA_ARGS__)

class UInputAction;
class UInputMappingContext;
class UALSGameInstance;
class AALSGameMode;

/**
 * 玩家输入系统
 */
USTRUCT(BlueprintType)
struct FPlayerInputSystem {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	// 瞄准
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Aim;

	// 攻击长按
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_AttackHold;

	// 攻击点按
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_AttackTap;

	// 蹲伏
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Crouch;

	// 视角
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Look;

	// 移动
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Move;

	// 跳跃
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Jump;

	// 装备/解备手枪
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Pistol;

	// 布娃娃系统
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Ragdoll;

	// 装弹
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Reload;

	// 装备/解备激枪
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Rifle;

	// 翻滚
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Roll;

	// 游戏设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Settings;

	// 冲刺
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Sprint;

	// 行走切换
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> IA_Walk;
};

/**
 * 调试输入系统
 */
USTRUCT(BlueprintType)
struct FDebugInputSystem {
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugFocusedCharacterCycleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugOpenOverlayMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugOverlayMenuCycleAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleCharacterInfoAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleMeshAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleDebugViewAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleHudAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleLayerColorsAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleShapesAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleSlomoAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> DebugToggleTracesAction;
};

/**
 * 角色状态系统
 */
USTRUCT(BlueprintType)
struct FCharacterStateSystem {
	GENERATED_BODY()

	// 最大生命值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHP = 0.f;

	// 当前生命值
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float HP = 0.f;
};

/**
 * 帧率限制
 */
UENUM(BlueprintType)
enum class EFrameRateLimit : uint8 {
	Infinite,
	FPS60,
	FPS90,
	FPS120,
};

/**
 * 图形质量
 */
UENUM(BlueprintType)
enum class EGraphicsQuality {
	Low,
	Medium,
	High,
	Ultra,
	Extreme,
	Custom = -1,
};

/**
 * 图形设置
 */
USTRUCT(BlueprintType)
struct FGraphicsSettings {
	GENERATED_BODY()

	static const FString GraphicsSettingsGraphicsQuality_Custom;
	static const FString GraphicsSettingsGraphicsQuality_Low;
	static const FString GraphicsSettingsGraphicsQuality_Medium;
	static const FString GraphicsSettingsGraphicsQuality_High;
	static const FString GraphicsSettingsGraphicsQuality_Ultra;
	static const FString GraphicsSettingsGraphicsQuality_Extreme;

	static const FString GraphicsSettingsWindowMode_Windowed;
	static const FString GraphicsSettingsWindowMode_FullScreen;
	static const FString GraphicsSettingsWindowMode_WindowedFullScreen;

	static const FString GraphicsSettingsFrameRateLimit_60;
	static const FString GraphicsSettingsFrameRateLimit_90;
	static const FString GraphicsSettingsFrameRateLimit_120;
	static const FString GraphicsSettingsFrameRateLimit_Infinite;

	// 垂直同步
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool VSync = false;

	// 显示FPS
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool ShowFPS = false;

	// 分辨率
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FIntPoint Resolution{ 1920, 1080 };

	// 窗口模式
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TEnumAsByte<EWindowMode::Type> WindowMode{ EWindowMode::Windowed };

	// 帧率限制
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EFrameRateLimit FrameRateLimit{ EFrameRateLimit::Infinite };

	// 图形质量
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality GraphicsQuality{ EGraphicsQuality::High };

	// 反射
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality ReflectionQuality{ EGraphicsQuality::High };

	// 后期处理
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality PostProcessingQuality{ EGraphicsQuality::High };

	// 抗锯齿
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality AntiAliasingQuality{ EGraphicsQuality::High };

	// 全局光照
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality GlobalIlluminationQuality{ EGraphicsQuality::High };

	// 特效
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality VisualEffectQuality{ EGraphicsQuality::High };

	// 视距
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality ViewDistanceQuality{ EGraphicsQuality::High };

	// 纹理
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality TextureQuality{ EGraphicsQuality::High };

	// 阴影
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality ShadowQuality{ EGraphicsQuality::High };

	// 着色
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality ShadingQuality{ EGraphicsQuality::High };

	// 植物
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	EGraphicsQuality FoliageQuality{ EGraphicsQuality::High };

	void SetResolution(const FString& Value) {
		TArray<FString> StringArray;
		if (Value.ParseIntoArray(StringArray, TEXT("*"), true) != 2) {
			ALS_FATAL(TEXT("Value is invalid: %s:%d"), __FILEW__, __LINE__);
		}

		LexFromString(Resolution.X, *StringArray[0]);
		LexFromString(Resolution.Y, *StringArray[1]);
	}

	FString GetResolutionString() const {
		return FString::Printf(TEXT("%d * %d"), Resolution.X, Resolution.Y);
	}

	void SetWindowMode(const FString& Value) {
		if (Value == GraphicsSettingsWindowMode_Windowed) {
			WindowMode = EWindowMode::Windowed;
		}
		else if (Value == GraphicsSettingsWindowMode_FullScreen) {
			WindowMode = EWindowMode::Fullscreen;
		}
		else if (Value == GraphicsSettingsWindowMode_WindowedFullScreen) {
			WindowMode = EWindowMode::WindowedFullscreen;
		}
		else {
			ALS_FATAL(TEXT("Value is invalid: %s:%d"), __FILEW__, __LINE__);
		}
	}

	FString GetWindowModeString() const {
		switch (WindowMode) {
		case EWindowMode::Fullscreen: return GraphicsSettingsWindowMode_FullScreen;
		case EWindowMode::WindowedFullscreen: return GraphicsSettingsWindowMode_WindowedFullScreen;
		}

		return GraphicsSettingsWindowMode_Windowed;
	}

	void SetFrameRateLimit(const FString& Value) {
		if (Value == GraphicsSettingsFrameRateLimit_60) {
			FrameRateLimit = EFrameRateLimit::FPS60;
		}
		else if (Value == GraphicsSettingsFrameRateLimit_90) {
			FrameRateLimit = EFrameRateLimit::FPS90;
		}
		else if (Value == GraphicsSettingsFrameRateLimit_120) {
			FrameRateLimit = EFrameRateLimit::FPS120;
		}
		else if (Value == GraphicsSettingsFrameRateLimit_Infinite) {
			FrameRateLimit = EFrameRateLimit::Infinite;
		}
		else {
			ALS_FATAL(TEXT("Value is invalid: %s:%d"), __FILEW__, __LINE__);
		}
	}

	float GetFrameRateLimitValue() const {
		switch (FrameRateLimit) {
		case EFrameRateLimit::FPS60: return 60.f;
		case EFrameRateLimit::FPS90: return 90.f;
		case EFrameRateLimit::FPS120: return 120.f;
		}

		return 0.f;
	}

	FString GetFrameRateLimitString() const {
		switch (FrameRateLimit) {
		case EFrameRateLimit::FPS60: return GraphicsSettingsFrameRateLimit_60;
		case EFrameRateLimit::FPS90: return GraphicsSettingsFrameRateLimit_90;
		case EFrameRateLimit::FPS120: return GraphicsSettingsFrameRateLimit_120;
		}

		return GraphicsSettingsFrameRateLimit_Infinite;
	}

	void SetGraphicQuality(const FString& Value) {
		if (Value == GraphicsSettingsGraphicsQuality_Low) {
			GraphicsQuality = EGraphicsQuality::Low;
		}
		else if (Value == GraphicsSettingsGraphicsQuality_Medium) {
			GraphicsQuality = EGraphicsQuality::Medium;
		}
		else if (Value == GraphicsSettingsGraphicsQuality_High) {
			GraphicsQuality = EGraphicsQuality::High;
		}
		else if (Value == GraphicsSettingsGraphicsQuality_Ultra) {
			GraphicsQuality = EGraphicsQuality::Ultra;
		}
		else if (Value == GraphicsSettingsGraphicsQuality_Extreme) {
			GraphicsQuality = EGraphicsQuality::Extreme;
		}
		else if (Value == GraphicsSettingsGraphicsQuality_Custom) {
			GraphicsQuality = EGraphicsQuality::Custom;
		}
		else {
			ALS_FATAL(TEXT("Value is invalid: %s:%d"), __FILEW__, __LINE__);
		}
	}

	FString GetGraphicsQualityString() const {
		switch (GraphicsQuality) {
		case EGraphicsQuality::Low: return GraphicsSettingsGraphicsQuality_Low;
		case EGraphicsQuality::Medium: return GraphicsSettingsGraphicsQuality_Medium;
		case EGraphicsQuality::High: return GraphicsSettingsGraphicsQuality_High;
		case EGraphicsQuality::Ultra: return GraphicsSettingsGraphicsQuality_Ultra;
		case EGraphicsQuality::Extreme: return GraphicsSettingsGraphicsQuality_Extreme;
		}

		return GraphicsSettingsGraphicsQuality_Custom;
	}

	FString GetGraphicsQualityString(EGraphicsQuality Value) const {
		switch (Value) {
		case EGraphicsQuality::Low: return GraphicsSettingsGraphicsQuality_Low;
		case EGraphicsQuality::Medium: return GraphicsSettingsGraphicsQuality_Medium;
		case EGraphicsQuality::High: return GraphicsSettingsGraphicsQuality_High;
		case EGraphicsQuality::Ultra: return GraphicsSettingsGraphicsQuality_Ultra;
		case EGraphicsQuality::Extreme: return GraphicsSettingsGraphicsQuality_Extreme;
		}

		return GraphicsSettingsGraphicsQuality_Custom;
	}

	void SetReflectionQuality(float Value) {
		ReflectionQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetReflectionQuality() const {
		return (float)ReflectionQuality;
	}

	void SetPostProcessingQuality(float Value) {
		PostProcessingQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetPostProcessingQuality() const {
		return (float)PostProcessingQuality;
	}

	void SetAntiAliasingQuality(float Value) {
		AntiAliasingQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetAntiAliasingQuality() const {
		return (float)AntiAliasingQuality;
	}

	void SetGlobalIlluminationQuality(float Value) {
		GlobalIlluminationQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetGlobalIlluminationQuality() const {
		return (float)GlobalIlluminationQuality;
	}

	void SetVisualEffectQuality(float Value) {
		VisualEffectQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetVisualEffectQuality() const {
		return (float)VisualEffectQuality;
	}

	void SetViewDistanceQuality(float Value) {
		ViewDistanceQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetViewDistanceQuality() const {
		return (float)ViewDistanceQuality;
	}

	void SetTextureQuality(float Value) {
		TextureQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetTextureQuality() const {
		return (float)TextureQuality;
	}

	void SetShadowQuality(float Value) {
		ShadowQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetShadowQuality() const {
		return (float)ShadowQuality;
	}

	void SetShadingQuality(float Value) {
		ShadingQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetShadingQuality() const {
		return (float)ShadingQuality;
	}

	void SetFoliageQuality(float Value) {
		FoliageQuality = EGraphicsQuality((int)(Value + 0.5f));
	}

	float GetFoliageQuality() const {
		return (float)FoliageQuality;
	}
};

/**
 * 游戏设置
 */
USTRUCT(BlueprintType)
struct FGameSettings {
	GENERATED_BODY()

	static const FGameSettings DefaultGameSettings;

	// 图形设置
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FGraphicsSettings GraphicsSettings;
};

/**
 * ALS库
 */
UCLASS()
class ALSV4_CPP_API UALSLibrary : public UObject {
	GENERATED_BODY()

public:
	static UALSLibrary* Instance() {
		static UALSLibrary* instance = nullptr;

		if (!instance) {
			instance = NewObject<UALSLibrary>();
		}

		return instance;
	}

	UWorld* GetCurrentWorld() const;
	UALSGameInstance* GetGameInstance(UWorld* World = nullptr) const;
	AALSGameMode* GetGameMode(UWorld* World = nullptr) const;
	
private:
	UALSLibrary() : Super() {}
};
