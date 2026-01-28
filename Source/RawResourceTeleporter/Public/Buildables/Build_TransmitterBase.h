// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"
#include "Build_TransmitterBase.generated.h"

class UHologramOverrides;

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ABuild_TransmitterBase : public AFGBuildableFactory
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_TransmitterBase();

	//Mediator function to pass control from the C++ only hologram to the data asset.
	UHologramOverrides* GetHologramOverride() const;

private:
	UPROPERTY(EditDefaultsOnly)
	UFGFactoryConnectionComponent* inputConnection = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UHologramOverrides* HologramOverrides;
};
