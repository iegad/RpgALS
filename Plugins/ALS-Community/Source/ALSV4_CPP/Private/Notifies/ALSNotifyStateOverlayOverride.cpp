#include "Notifies/ALSNotifyStateOverlayOverride.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSNotifyStateOverlayOverride)

#include "Character/ALSBaseCharacter.h"
#include "Character/Animation/ALSCharacterAnimInstance.h"

void 
UALSNotifyStateOverlayOverride::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                                 float TotalDuration, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		Character->SetOverlayOverrideState(OverlayOverrideState);
	}
}

void 
UALSNotifyStateOverlayOverride::NotifyEnd(
	USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) {
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AALSBaseCharacter* Character = Cast<AALSBaseCharacter>(MeshComp->GetOwner());
	if (Character) {
		Character->SetOverlayOverrideState(0);
	}
}

FString 
UALSNotifyStateOverlayOverride::GetNotifyName_Implementation() const {
	FString Name(TEXT("Overlay Override State: "));
	Name.Append(FString::FromInt(OverlayOverrideState));
	return Name;
}