// Copyright:	Copyright (C) 2024 iegad

#include "Weapons/Guns/XBaseGun.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(XBaseGun)

#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"

#include "Bases/XLibrary.h"
#include "Components/XGunSystemComponent.h"
#include "DataAssets/XGunDataAsset.h"

AXBaseGun::AXBaseGun() : Super() {
	// Create StaticMesh
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName{ TEXTVIEW("StaticMesh") });
	check(StaticMesh);
	SetRootComponent(StaticMesh);

	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	StaticMesh->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

	FCollisionResponseContainer StaticMeshCRC;
	StaticMeshCRC.SetResponse(ECC_WorldStatic, ECR_Block);
	StaticMeshCRC.SetResponse(ECC_WorldDynamic, ECR_Block);
	StaticMeshCRC.SetResponse(ECC_Pawn, ECR_Ignore);
	StaticMeshCRC.SetResponse(ECC_Visibility, ECR_Ignore);
	StaticMeshCRC.SetResponse(ECC_Camera, ECR_Ignore);
	StaticMeshCRC.SetResponse(ECC_PhysicsBody, ECR_Block);
	StaticMeshCRC.SetResponse(ECC_Vehicle, ECR_Ignore);
	StaticMeshCRC.SetResponse(ECC_Destructible, ECR_Block);
	StaticMesh->SetCollisionResponseToChannels(StaticMeshCRC);

	// Create BoxCollision
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(FName{ TEXTVIEW("BoxCollision") });
	check(BoxCollision);
	BoxCollision->SetupAttachment(StaticMesh);
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
	StaticMesh->SetSimulatePhysics(false);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void 
AXBaseGun::EnableCollision() {
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void 
AXBaseGun::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	LoadData();
}

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

void
AXBaseGun::LoadData() {
	if (DataAsset) {
		StaticMesh->SetStaticMesh(DataAsset->Mesh);
		StaticMesh->BodyInstance.SetMassOverride(DataAsset->Mass);
	}
	else {
		StaticMesh->SetStaticMesh(nullptr);
		StaticMesh->BodyInstance.SetMassOverride(0.f, false);
	}
}
