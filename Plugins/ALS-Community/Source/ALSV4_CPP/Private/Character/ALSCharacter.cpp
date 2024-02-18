// Copyright:       Copyright (C) 2022 Doğa Can Yanıkoğlu
// Source Code:     https://github.com/dyanikoglu/ALS-Community


#include "Character/ALSCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "AI/ALSAIController.h"
#include "Kismet/GameplayStatics.h"
#include "Props/BowAnim.h"
#include "Props/PropsBase.h"
#include "Components/WeaponComponent.h"

AALSCharacter::AALSCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	HeldObjectRoot = CreateDefaultSubobject<USceneComponent>(TEXT("HeldObjectRoot"));
	HeldObjectRoot->SetupAttachment(GetMesh());

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(HeldObjectRoot);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(HeldObjectRoot);

	AIControllerClass = AALSAIController::StaticClass();

	CreatePropsSystem();
	CreateCustomComponent();
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

APropsBase*
AALSCharacter::GetCurrentProps() const {
	APropsBase* CurrentProps = nullptr;

	switch (GetOverlayState()) {
	case EALSOverlayState::Rifle: 
		CurrentProps = Cast<APropsBase>(ChildActorRifle->GetChildActor());
		break;

	case EALSOverlayState::PistolOneHanded: 
		CurrentProps = Cast<APropsBase>(ChildActorPistol->GetChildActor());
		break;

	case EALSOverlayState::PistolTwoHanded: 
		CurrentProps = Cast<APropsBase>(ChildActorPistol->GetChildActor());
		break;

	default: break;
	}

	return CurrentProps;
}

APropsBase* 
AALSCharacter::GetPropsFromOverlayState(EALSOverlayState Overlay) const {
	APropsBase* Props = nullptr;

	switch (Overlay) {
	case EALSOverlayState::Rifle:
		Props = Cast<APropsBase>(ChildActorRifle->GetChildActor());
		break;
		
	case EALSOverlayState::PistolOneHanded:
		Props = Cast<APropsBase>(ChildActorPistol->GetChildActor());
		break;

	default:
		break;
	}

	return Props;
}

void AALSCharacter::OnOverlayStateChanged(EALSOverlayState PreviousState)
{
	Super::OnOverlayStateChanged(PreviousState);
	UpdateHeldObject();
}

void 
AALSCharacter::RifleAction_Implementation() {
	do {
		APropsBase* Props = Cast<APropsBase>(ChildActorRifle->GetChildActor());
		if (!Props || Props->OverlayState < EALSOverlayState::Rifle) {
			break;
		}

		if (GetOverlayState() == Props->OverlayState) {
			WeaponComponent->UnEquip(this, Props);
			break;
		}

		WeaponComponent->Equip(this, Props);
	} while (0);
}

void 
AALSCharacter::PistolAction_Implementation() {
	do {
		APropsBase* Props = Cast<APropsBase>(ChildActorPistol->GetChildActor());
		if (!Props || Props->OverlayState < EALSOverlayState::Rifle) {
			break;
		}

		if (GetOverlayState() == Props->OverlayState) {
			WeaponComponent->UnEquip(this, Props);
			break;
		}

		WeaponComponent->Equip(this, Props);
	} while (0);
}

void AALSCharacter::UpdateHeldObjectAnimations()
{
	if (GetOverlayState() == EALSOverlayState::Bow) {
		UBowAnim* BowAnim = Cast<UBowAnim>(SkeletalMesh->GetAnimInstance());
		if (BowAnim) {
			BowAnim->Draw = GetAnimCurveValue(TEXT("Enable_SpineRotation"));
		}
	}
}

inline void 
AALSCharacter::CreatePropsSystem() {
	SceneRifle = CreateDefaultSubobject<USceneComponent>(TEXT("ScnRifle"));
	SceneRifle->SetupAttachment(GetMesh(), TEXT("Slot_Rifle"));

	ScenePistol = CreateDefaultSubobject<USceneComponent>(TEXT("ScnPistol"));
	ScenePistol->SetupAttachment(GetMesh(), TEXT("Slot_Pistol"));

	ChildActorRifle = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorRifle"));
	ChildActorRifle->SetupAttachment(SceneRifle);

	ChildActorPistol = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildActorPistol"));
	ChildActorPistol->SetupAttachment(ScenePistol);
}

inline void 
AALSCharacter::CreateCustomComponent() {
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("WeaponComponent"));
}

void AALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHeldObjectAnimations();
}

void AALSCharacter::BeginPlay()
{
	Super::BeginPlay();

	UpdateHeldObject();
}
