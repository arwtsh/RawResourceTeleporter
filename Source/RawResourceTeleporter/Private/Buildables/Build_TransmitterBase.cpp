// 


#include "Build_TransmitterBase.h"
#include "Hologram/HologramOverrides.h"
#include "RawResourceTeleporter.h"
#include "FGFactoryConnectionComponent.h"


// Sets default values
ABuild_TransmitterBase::ABuild_TransmitterBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	inputConnection = CreateDefaultSubobject<UFGFactoryConnectionComponent>(TEXT("inputConnector"));
	inputConnection->SetupAttachment(RootComponent);
	inputConnection->SetDirection(EFactoryConnectionDirection::FCD_SNAP_ONLY);
}

UHologramOverrides* ABuild_TransmitterBase::GetHologramOverride() const
{
	if (!HologramOverrides)
	{
		UE_LOG(LogRawResourceTeleporter, Error, TEXT("Forgot to assign the HologramOverrides asset to Transmitter asset %s"), *StaticClass()->GetName());
		return {};
	}
	
	return HologramOverrides;
}


