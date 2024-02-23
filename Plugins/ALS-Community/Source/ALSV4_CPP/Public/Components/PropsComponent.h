// Copyright:       Copyright (C) 2024 iegad
// Source Code:     https://github.com/iegad/RpgALS

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PropsComponent.generated.h"

class AALSCharacter;
class APropsBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ALSV4_CPP_API UPropsComponent : public UActorComponent {
	GENERATED_BODY()

public:	
	UPropsComponent();

public:
	UFUNCTION(BlueprintCallable)
	void StartEquip(APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void EndEquip(APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void StartUnEquip(APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void EndUnEquip(APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void Equip(APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void UnEquip(APropsBase* Props) const;

protected:
	inline AALSCharacter* GetALSCharacter() const;

private:
	mutable APropsBase* DesiredProps = nullptr;
};
