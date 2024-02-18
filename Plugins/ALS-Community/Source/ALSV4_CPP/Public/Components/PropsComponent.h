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
	void StartEquip(AALSCharacter* Character, const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void EndEquip(AALSCharacter* Character, const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void StartUnEquip(AALSCharacter* Character, const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void EndUnEquip(AALSCharacter* Character, const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void Equip(AALSCharacter* Character, const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void UnEquip(AALSCharacter* Character, const APropsBase* Props) const;

private:
	mutable APropsBase* DesiredProps = nullptr;
};
