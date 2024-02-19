#include "Props/PropsBase.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(PropsBase)

#include "ALSLibrary.h"

APropsBase::APropsBase() : Super() {
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
}

void 
APropsBase::BeginPlay() {
	Super::BeginPlay();

	if (EquipSocketName.IsNone()) {
		ALS_ERROR(TEXT("%s's EquipSocketName is invalid"), *this->GetClass()->GetName());
	}

	if (!EquipMontage) {
		ALS_ERROR(TEXT("%s's EquipMontage is nullptr: %s:%d"), *this->GetClass()->GetName(), __FILEW__, __LINE__);
	}

	if (!UnEquipMontage) {
		ALS_ERROR(TEXT("%s's UnEquipMontage is nullptr: %s:%d"), *this->GetClass()->GetName(), __FILEW__, __LINE__);
	}
}
