#pragma once
#include "CoreMinimal.h"
#include "Hologram/FGHologramBuildModeDescriptor.h"
#include "TransmitterBuildModes.generated.h"

UCLASS()
class RAWRESOURCETELEPORTER_API USolidTransmitterBuildMode : public UFGHologramBuildModeDescriptor
{
	GENERATED_BODY()
	
public:
	USolidTransmitterBuildMode()
	{
		//TODO-Localize this text.
		mDisplayName = INVTEXT("Solid");
	}
};

UCLASS()
class RAWRESOURCETELEPORTER_API UFluidTransmitterBuildMode : public UFGHologramBuildModeDescriptor
{
	GENERATED_BODY()
	
public:
	UFluidTransmitterBuildMode()
	{
		//TODO-Localize this text.
		mDisplayName = INVTEXT("Fluid");
	}
};
