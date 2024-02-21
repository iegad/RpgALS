// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community
// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "Core/ALSPlayerController.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSPlayerController)

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"

#include "AI/ALSAIController.h"
#include "CameraSystem/ALSPlayerCameraManager.h"
#include "Character/ALSCharacter.h"
#include "Components/ALSDebugComponent.h"

#include "UI/ALSGameSettings.h"

void 
AALSPlayerController::OnPossess(APawn* NewPawn) {
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<AALSBaseCharacter>(NewPawn);
	if (!IsRunningDedicatedServer()) {
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}

	SetupInputs();

	if (!IsValid(PossessedCharacter)) {
		return;
	}
	
	UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
	if (DebugComp) {
		DebugComp->OnPlayerControllerInitialized(this);
	}

	if (ALSGameSettingsClass) {
		UI_ALSGameSettings = CreateWidget<UALSGameSettings>(GetGameInstance(), ALSGameSettingsClass);
	}
}

void 
AALSPlayerController::OnRep_Pawn() {
	Super::OnRep_Pawn();
	PossessedCharacter = Cast<AALSBaseCharacter>(GetPawn());
	SetupCamera();
	SetupInputs();
	
	if (!PossessedCharacter) {
		return;
	}

	UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
	if (DebugComp) {
		DebugComp->OnPlayerControllerInitialized(this);
	}
}

void 
AALSPlayerController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) {
		UE_LOG(LogTemp, Fatal, TEXT("ALS Community requires Enhanced Input System to be activated in project settings to function properly"));
		return;
	}

	EnhancedInputComponent->ClearActionEventBindings();
	EnhancedInputComponent->ClearActionValueBindings();
	EnhancedInputComponent->ClearDebugKeyBindings();

	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Aim, ETriggerEvent::Completed, this, &AALSPlayerController::IA_Aim);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_AttackHold, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_AttackHold);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_AttackTap, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_AttackTap);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Crouch, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Crouch);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Look, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Look);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Move, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Move);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Jump, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Jump);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Pistol, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Pistol);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Ragdoll, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Ragdoll);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Rifle, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Rifle);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Roll, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Roll);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Sprint, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Sprint);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Walk, ETriggerEvent::Triggered, this, &AALSPlayerController::IA_Walk);
	EnhancedInputComponent->BindAction(PlayerInputSystem.IA_Settings, ETriggerEvent::Completed, this, &AALSPlayerController::IA_Settings);

	EnhancedInputComponent->BindAction(DebugInputSystem.DebugFocusedCharacterCycleAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugFocusedCharacterCycleAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugOpenOverlayMenuAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugOpenOverlayMenuAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugOverlayMenuCycleAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugOverlayMenuCycleAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleCharacterInfoAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleCharacterInfoAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleMeshAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleMeshAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleDebugViewAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleDebugViewAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleHudAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleHudAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleLayerColorsAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleLayerColorsAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleShapesAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleShapesAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleSlomoAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleSlomoAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleTracesAction, ETriggerEvent::Triggered, this, &AALSPlayerController::DebugToggleTracesAction);
}

void 
AALSPlayerController::SetupInputs() {
	if (PossessedCharacter) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
			FModifyContextOptions Options;
			Options.bForceImmediately = 1;
			Subsystem->AddMappingContext(PlayerInputSystem.InputMappingContext, 1, Options);
			UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
			if (DebugComp) {
				// Do only if we have debug component
				Subsystem->AddMappingContext(DebugInputSystem.InputMappingContext, 0, Options);
			}
		}
	}
}

void 
AALSPlayerController::SetupCamera() {
	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	AALSPlayerCameraManager* CastedMgr = Cast<AALSPlayerCameraManager>(PlayerCameraManager);
	if (PossessedCharacter && CastedMgr) {
		CastedMgr->OnPossess(PossessedCharacter);
	}
}

void 
AALSPlayerController::SetGlobalTimeDilationLocal(float Dilation) {
	UWorld* World = GetWorld();

	if (World && UKismetSystemLibrary::IsStandalone(World)) {
		UGameplayStatics::SetGlobalTimeDilation(World, Dilation);
	}
}

void
AALSPlayerController::IA_Move(const FInputActionValue& Value) {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Move(Value);
	}
}

void 
AALSPlayerController::IA_Look(const FInputActionValue& Value) {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Look(Value);
	}
}

void 
AALSPlayerController::IA_Jump(const FInputActionValue& Value) {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Jump(Value);
	}
}

void 
AALSPlayerController::IA_Sprint(const FInputActionValue& Value) {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Sprint(Value);
	}
}

void 
AALSPlayerController::IA_Aim() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Aim();
	}
}

void 
AALSPlayerController::IA_Crouch() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Crouch();
	}
}

void 
AALSPlayerController::IA_Walk() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Walk();
	}
}

void 
AALSPlayerController::IA_Ragdoll() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Ragdoll();
	}
}

void 
AALSPlayerController::IA_Rifle() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Rifle();
	}
}

void
AALSPlayerController::IA_Roll() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Roll();
	}
}

void 
AALSPlayerController::IA_Pistol() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_Pistol();
	}
}

void 
AALSPlayerController::IA_AttackHold() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_AttackHold();
	}
}

void 
AALSPlayerController::IA_AttackTap() {
	if (PossessedCharacter) {
		PossessedCharacter->IA_AttackTap();
	}
}

void 
AALSPlayerController::IA_Settings() {
	if (!UI_ALSGameSettings) {
		return;
	}

	if (!UI_ALSGameSettings->IsInViewport()) {
		UI_ALSGameSettings->AddToViewport();

		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		InputMode.SetHideCursorDuringCapture(false);
		InputMode.SetWidgetToFocus(UI_ALSGameSettings->TakeWidget());
		SetInputMode(InputMode);
		SetShowMouseCursor(true);
		return;
	}

	if (UI_ALSGameSettings->IsVisible()) {
		UI_ALSGameSettings->SetVisibility(ESlateVisibility::Hidden);
		SetInputMode(FInputModeGameOnly{});
		SetShowMouseCursor(false);
		return;
	}

	UI_ALSGameSettings->SetVisibility(ESlateVisibility::Visible);
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetWidgetToFocus(UI_ALSGameSettings->TakeWidget());
	SetInputMode(InputMode);
	SetShowMouseCursor(true);
}

void 
AALSPlayerController::DebugToggleHudAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleHud();
		}
	}
}

void 
AALSPlayerController::DebugToggleDebugViewAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleDebugView();
		}
	}
}

void 
AALSPlayerController::DebugToggleTracesAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleTraces();
		}
	}
}

void 
AALSPlayerController::DebugToggleShapesAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleDebugShapes();
		}
	}
}

void 
AALSPlayerController::DebugToggleLayerColorsAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleLayerColors();
		}
	}
}

void 
AALSPlayerController::DebugToggleCharacterInfoAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleCharacterInfo();
		}
	}
}

void 
AALSPlayerController::DebugToggleSlomoAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleSlomo();
		}
	}
}

void 
AALSPlayerController::DebugFocusedCharacterCycleAction(const FInputActionValue& Value) {
	if (PossessedCharacter) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->FocusedDebugCharacterCycle(Value.GetMagnitude() > 0);
		}
	}
}

void 
AALSPlayerController::DebugToggleMeshAction(const FInputActionValue& Value) {
	if (PossessedCharacter && Value.Get<bool>()) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->ToggleDebugMesh();
		}
	}
}

void 
AALSPlayerController::DebugOpenOverlayMenuAction(const FInputActionValue& Value) {
	if (PossessedCharacter) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->OpenOverlayMenu(Value.Get<bool>());
		}
	}
}

void 
AALSPlayerController::DebugOverlayMenuCycleAction(const FInputActionValue& Value) {
	if (PossessedCharacter) {
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp) {
			DebugComp->OverlayMenuCycle(Value.GetMagnitude() > 0);
		}
	}
}
