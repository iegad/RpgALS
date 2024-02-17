// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Character/ALSPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Engine/LocalPlayer.h"
#include "AI/ALSAIController.h"
#include "Character/ALSCharacter.h"
#include "Character/ALSPlayerCameraManager.h"
#include "Components/ALSDebugComponent.h"
#include "Kismet/GameplayStatics.h"

void AALSPlayerController::OnPossess(APawn* NewPawn)
{
	Super::OnPossess(NewPawn);
	PossessedCharacter = Cast<AALSBaseCharacter>(NewPawn);
	if (!IsRunningDedicatedServer())
	{
		// Servers want to setup camera only in listen servers.
		SetupCamera();
	}

	SetupInputs();

	if (!IsValid(PossessedCharacter)) return;
	
	UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
	if (DebugComp)
	{
		DebugComp->OnPlayerControllerInitialized(this);
	}
}

void AALSPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();
	PossessedCharacter = Cast<AALSBaseCharacter>(GetPawn());
	SetupCamera();
	SetupInputs();
	
	if (!PossessedCharacter) return;

	UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
	if (DebugComp)
	{
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

	EnhancedInputComponent->BindAction(PlayerInputSystem.AimAction, ETriggerEvent::Completed, this, &ThisClass::AimAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.CameraHeldAction, ETriggerEvent::Triggered, this, &ThisClass::CameraHeldAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.CameraRightAction, ETriggerEvent::Triggered, this, &ThisClass::CameraRightAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.CameraTapAction, ETriggerEvent::Triggered, this, &ThisClass::CameraTapAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.CameraUpAction, ETriggerEvent::Triggered, this, &ThisClass::CameraUpAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.ForwardMovementAction, ETriggerEvent::Triggered, this, &ThisClass::ForwardMovementAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.JumpAction, ETriggerEvent::Triggered, this, &ThisClass::JumpAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.LookingDirectionAction, ETriggerEvent::Triggered, this, &ThisClass::LookingDirectionAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.RagdollAction, ETriggerEvent::Triggered, this, &ThisClass::RagdollAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.RightMovementAction, ETriggerEvent::Triggered, this, &ThisClass::RightMovementAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.SprintAction, ETriggerEvent::Triggered, this, &ThisClass::SprintAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.StanceAction, ETriggerEvent::Triggered, this, &ThisClass::StanceAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.VelocityDirectionAction, ETriggerEvent::Triggered, this, &ThisClass::VelocityDirectionAction);
	EnhancedInputComponent->BindAction(PlayerInputSystem.WalkAction, ETriggerEvent::Triggered, this, &ThisClass::WalkAction);

	EnhancedInputComponent->BindAction(DebugInputSystem.DebugFocusedCharacterCycleAction, ETriggerEvent::Triggered, this, &ThisClass::DebugFocusedCharacterCycleAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugOpenOverlayMenuAction, ETriggerEvent::Triggered, this, &ThisClass::DebugOpenOverlayMenuAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugOverlayMenuCycleAction, ETriggerEvent::Triggered, this, &ThisClass::DebugOverlayMenuCycleAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleCharacterInfoAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleCharacterInfoAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleMeshAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleMeshAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleDebugViewAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleDebugViewAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleHudAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleHudAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleLayerColorsAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleLayerColorsAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleShapesAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleShapesAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleSlomoAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleSlomoAction);
	EnhancedInputComponent->BindAction(DebugInputSystem.DebugToggleTracesAction, ETriggerEvent::Triggered, this, &ThisClass::DebugToggleTracesAction);
}

void AALSPlayerController::SetupInputs()
{
	if (PossessedCharacter)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
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

void AALSPlayerController::SetupCamera()
{
	// Call "OnPossess" in Player Camera Manager when possessing a pawn
	AALSPlayerCameraManager* CastedMgr = Cast<AALSPlayerCameraManager>(PlayerCameraManager);
	if (PossessedCharacter && CastedMgr)
	{
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

void AALSPlayerController::ForwardMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->ForwardMovementAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::RightMovementAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->RightMovementAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::CameraUpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraUpAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::CameraRightAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraRightAction(Value.GetMagnitude());
	}
}

void AALSPlayerController::JumpAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->JumpAction(Value.Get<bool>());
	}
}

void AALSPlayerController::SprintAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->SprintAction(Value.Get<bool>());
	}
}

void AALSPlayerController::AimAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->AimAction(Value.Get<bool>());
	}
}

void AALSPlayerController::CameraTapAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraTapAction();
	}
}

void AALSPlayerController::CameraHeldAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		PossessedCharacter->CameraHeldAction();
	}
}

void AALSPlayerController::StanceAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->StanceAction();
	}
}

void AALSPlayerController::WalkAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->WalkAction();
	}
}

void AALSPlayerController::RagdollAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->RagdollAction();
	}
}

void AALSPlayerController::VelocityDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->VelocityDirectionAction();
	}
}

void AALSPlayerController::LookingDirectionAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		PossessedCharacter->LookingDirectionAction();
	}
}

void AALSPlayerController::DebugToggleHudAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleHud();
		}
	}
}

void AALSPlayerController::DebugToggleDebugViewAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugView();
		}
	}
}

void AALSPlayerController::DebugToggleTracesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleTraces();
		}
	}
}

void AALSPlayerController::DebugToggleShapesAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugShapes();
		}
	}
}

void AALSPlayerController::DebugToggleLayerColorsAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleLayerColors();
		}
	}
}

void AALSPlayerController::DebugToggleCharacterInfoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleCharacterInfo();
		}
	}
}

void AALSPlayerController::DebugToggleSlomoAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleSlomo();
		}
	}
}

void AALSPlayerController::DebugFocusedCharacterCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->FocusedDebugCharacterCycle(Value.GetMagnitude() > 0);
		}
	}
}

void AALSPlayerController::DebugToggleMeshAction(const FInputActionValue& Value)
{
	if (PossessedCharacter && Value.Get<bool>())
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->ToggleDebugMesh();
		}
	}
}

void AALSPlayerController::DebugOpenOverlayMenuAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OpenOverlayMenu(Value.Get<bool>());
		}
	}
}

void AALSPlayerController::DebugOverlayMenuCycleAction(const FInputActionValue& Value)
{
	if (PossessedCharacter)
	{
		UALSDebugComponent* DebugComp = Cast<UALSDebugComponent>(PossessedCharacter->GetComponentByClass(UALSDebugComponent::StaticClass()));
		if (DebugComp)
		{
			DebugComp->OverlayMenuCycle(Value.GetMagnitude() > 0);
		}
	}
}
