#include "ALSActor.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(ALSActor)

// Sets default values
AALSActor::AALSActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AALSActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AALSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

