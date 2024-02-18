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
	void StartEquip(const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void EndEquip(const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void StartUnEquip(const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void EndUnEquip(const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void Equip(const APropsBase* Props) const;

	UFUNCTION(BlueprintCallable)
	void UnEquip(const APropsBase* Props) const;

protected:
	inline AALSCharacter* GetALSCharacter() const;

private:
	mutable APropsBase* DesiredProps = nullptr;
};
