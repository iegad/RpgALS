// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community
// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#include "CameraSystem/ALSPlayerCameraBehavior.h"
#include "Character/ALSBaseCharacter.h"

void UALSPlayerCameraBehavior::SetRotationMode(EALSRotationMode RotationMode)
{
	bVelocityDirection = RotationMode == EALSRotationMode::VelocityDirection;
	bLookingDirection = RotationMode == EALSRotationMode::LookingDirection;
	bAiming = RotationMode == EALSRotationMode::Aiming;
}
