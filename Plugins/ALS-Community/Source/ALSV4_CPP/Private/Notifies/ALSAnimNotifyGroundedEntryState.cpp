// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community

#include "Notifies/ALSAnimNotifyGroundedEntryState.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSAnimNotifyGroundedEntryState)

#include "Character/ALSBaseCharacter.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"

void 
UALSAnimNotifyGroundedEntryState::Notify(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::Notify(MeshComp, Animation, EventReference);

	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		Character->SetGroundedEntryState(GroundedEntryState);
	}
}

FString 
UALSAnimNotifyGroundedEntryState::GetNotifyName_Implementation() const {
	FString Name(TEXT("Grounded Entry State: "));
	Name.Append(GetEnumerationToString(GroundedEntryState));
	return Name;
}
