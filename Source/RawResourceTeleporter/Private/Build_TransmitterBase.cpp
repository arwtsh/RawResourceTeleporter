// 


#include "Build_TransmitterBase.h"


// Sets default values
ABuild_TransmitterBase::ABuild_TransmitterBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuild_TransmitterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuild_TransmitterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

