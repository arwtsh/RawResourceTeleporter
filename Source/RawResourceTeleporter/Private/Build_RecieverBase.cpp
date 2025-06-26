// 


#include "Build_RecieverBase.h"


// Sets default values
ABuild_RecieverBase::ABuild_RecieverBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuild_RecieverBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuild_RecieverBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

