// 


#include "Build_TransmitterBase.h"

#include "FGFactoryConnectionComponent.h"


// Sets default values
ABuild_TransmitterBase::ABuild_TransmitterBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	inputConnection = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("inputConnector"));
}

