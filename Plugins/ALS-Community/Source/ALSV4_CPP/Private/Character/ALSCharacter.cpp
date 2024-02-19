#include "Character/ALSCharacter.h"

#include "Props/BowAnim.h"

AALSCharacter::AALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer) {
}

void 
AALSCharacter::UpdateHeldObject() {
	if (GetOverlayState() == EALSOverlayState::Default || 
		GetOverlayState() == EALSOverlayState::Masculine ||
		GetOverlayState() == EALSOverlayState::Feminine ||
		GetOverlayState() == EALSOverlayState::Injured || 
		GetOverlayState() == EALSOverlayState::HandsTied) {
		ClearHeldObject();
		return;
	}

	bool LeftHand = GetOverlayState() == EALSOverlayState::Bow ||
		GetOverlayState() == EALSOverlayState::Torch ||
		GetOverlayState() == EALSOverlayState::Barrel;

	USkeletalMesh* NewSkeletalMesh = SkeletalMeshMap.FindRef(GetOverlayState());
	UStaticMesh* NewStaticMesh = StaticMeshMap.FindRef(GetOverlayState());
	TSubclassOf<UAnimInstance> AnimClass = AnimClassMap.FindRef(GetOverlayState());

	AttachToHand(NewStaticMesh, NewSkeletalMesh, AnimClass, LeftHand, { 0, 0, 0 });
}

void AALSCharacter::ClearHeldObject()
{
	StaticMesh->SetStaticMesh(nullptr);
	SkeletalMesh->SetSkeletalMesh(nullptr);
	SkeletalMesh->SetAnimInstanceClass(nullptr);
}

void AALSCharacter::AttachToHand(UStaticMesh* NewStaticMesh, USkeletalMesh* NewSkeletalMesh, UClass* NewAnimClass, bool bLeftHand, FVector Offset)
{
	ClearHeldObject();

	//if (IsValid(NewStaticMesh))
	//{
	//	StaticMesh->SetStaticMesh(NewStaticMesh);
	//}
	//else if (IsValid(NewSkeletalMesh))
	//{
	//	SkeletalMesh->SetSkeletalMesh(NewSkeletalMesh);
	//	if (IsValid(NewAnimClass))
	//	{
	//		SkeletalMesh->SetAnimInstanceClass(NewAnimClass);
	//	}
	//}

	//FName AttachBone;
	//if (bLeftHand)
	//{
	//	AttachBone = TEXT("VB LHS_ik_hand_gun");
	//}
	//else
	//{
	//	AttachBone = TEXT("VB RHS_ik_hand_gun");
	//}

	//HeldObjectRoot->AttachToComponent(GetMesh(),
	//                                  FAttachmentTransformRules::SnapToTargetNotIncludingScale, AttachBone);
	//HeldObjectRoot->SetRelativeLocation(Offset);
}

void AALSCharacter::RagdollStart()
{
	ClearHeldObject();
	Super::RagdollStart();
}

void AALSCharacter::RagdollEnd()
{
	Super::RagdollEnd();
	UpdateHeldObject();
}

ECollisionChannel AALSCharacter::GetThirdPersonTraceParams(FVector& TraceOrigin, float& TraceRadius)
{
	const FName CameraSocketName = bRightShoulder ? TEXT("TP_CameraTrace_R") : TEXT("TP_CameraTrace_L");
	TraceOrigin = GetMesh()->GetSocketLocation(CameraSocketName);
	TraceRadius = 15.0f;
	return ECC_Camera;
}

FTransform AALSCharacter::GetThirdPersonPivotTarget()
{
	return FTransform(GetActorRotation(),
	                  (GetMesh()->GetSocketLocation(TEXT("Head")) + GetMesh()->GetSocketLocation(TEXT("root"))) / 2.0f,
	                  FVector::OneVector);
}

FVector AALSCharacter::GetFirstPersonCameraTarget()
{
	return GetMesh()->GetSocketLocation(TEXT("FP_Camera"));
}

void 
AALSCharacter::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	UpdateHeldObjectAnimations();
}

void 
AALSCharacter::BeginPlay() {
	Super::BeginPlay();
	UpdateHeldObject();
}

void
AALSCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState) {
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void 
AALSCharacter::UpdateHeldObjectAnimations() {
	if (GetOverlayState() == EALSOverlayState::Bow) {
		UBowAnim* BowAnim = Cast<UBowAnim>(SkeletalMesh->GetAnimInstance());
		if (BowAnim) {
			BowAnim->Draw = GetAnimCurveValue(TEXT("Enable_SpineRotation"));
		}
	}
}