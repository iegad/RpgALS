// Copyright:	Copyright (C) 2024 iegad

#include "Weapons/Guns/XBaseGun.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XBaseGun)

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"

#include "Components/XGunSystemComponent.h"

AXBaseGun::AXBaseGun() : Super() {
	// Create SkeletalMesh
	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(FName{ TEXTVIEW("StaticMesh") });
	XASSERT(SkeletalMesh, "CreateDefaultSubobject UStaticMeshComponent failed");
	SetRootComponent(SkeletalMesh);

	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SkeletalMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	FCollisionResponseContainer SkeletalMeshCRC;
	SkeletalMeshCRC.SetResponse(ECC_WorldStatic, ECR_Block);
	SkeletalMeshCRC.SetResponse(ECC_WorldDynamic, ECR_Block);
	SkeletalMeshCRC.SetResponse(ECC_Pawn, ECR_Ignore);
	SkeletalMeshCRC.SetResponse(ECC_Visibility, ECR_Ignore);
	SkeletalMeshCRC.SetResponse(ECC_Camera, ECR_Ignore);
	SkeletalMeshCRC.SetResponse(ECC_PhysicsBody, ECR_Block);
	SkeletalMeshCRC.SetResponse(ECC_Vehicle, ECR_Ignore);
	SkeletalMeshCRC.SetResponse(ECC_Destructible, ECR_Block);
	SkeletalMesh->SetCollisionResponseToChannels(SkeletalMeshCRC);

	// Create BoxCollision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName{ TEXTVIEW("BoxCollision") });
	XASSERT(BoxCollision, "CreateDefaultSubobject UBoxComponent failed");
	BoxCollision->SetupAttachment(SkeletalMesh);
	OnActorBeginOverlap.AddDynamic(this, &AXBaseGun::OnActorBeginOverlapHandler);
	OnActorEndOverlap.AddDynamic(this, &AXBaseGun::OnActorEndOverlapHandler);
}

bool 
AXBaseGun::Lock() {
	if (bLock) {
		return false;
	}

	bLock = true;
	return true;
}

void 
AXBaseGun::UnLock() {
	bLock = false;
}

void 
AXBaseGun::DisableCollision() {
	SkeletalMesh->SetSimulatePhysics(false);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void 
AXBaseGun::EnableCollision() {
	SkeletalMesh->SetSimulatePhysics(true);
	SkeletalMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

#if WITH_EDITOR
void 
AXBaseGun::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LoadData();
}
#endif

void
AXBaseGun::OnActorBeginOverlapHandler(AActor* OverlappedActor, AActor* OtherActor) {
	if (OtherActor && OtherActor->IsA<ACharacter>()) {
		UXGunSystemComponent* GunSystem = OtherActor->GetComponentByClass<UXGunSystemComponent>();
		if (GunSystem) {
			GunSystem->OnFocusGun.Broadcast(this);
		}
	}
}

void
AXBaseGun::OnActorEndOverlapHandler(AActor* OverlappedActor, AActor* OtherActor) {
	if (OtherActor && OtherActor->IsA<ACharacter>()) {
		UXGunSystemComponent* GunSystem = OtherActor->GetComponentByClass<UXGunSystemComponent>();
		if (GunSystem) {
			GunSystem->OnUnfocusGun.Broadcast(this);
		}
	}
}

bool 
AXBaseGun::LoadData() {
	if (!XGunDataTable) {
		return false;
	}

	if (RowID.IsNone()) {
		Data = nullptr;
		SkeletalMesh->SetSkeletalMesh(nullptr);
		SkeletalMesh->BodyInstance.SetMassOverride(0.f, false);
		return false;
	}
	
	Data = XGunDataTable->FindRow<FXGunData>(RowID, "");
	if (!Data) {
		XWARN("%s has no data: %s", *XGunDataTable->GetName(), *RowID.ToString());
		return false;
	}

	SkeletalMesh->SetSkeletalMesh(Data->Mesh);
	SkeletalMesh->BodyInstance.SetMassOverride(Data->Mass);
	return true;
}
