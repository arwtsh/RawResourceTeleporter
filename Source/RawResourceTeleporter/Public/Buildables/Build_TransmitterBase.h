// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"
#include "Build_TransmitterBase.generated.h"

class UHologramOverrides;

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ABuild_TransmitterBase : public AFGBuildable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_TransmitterBase();

	// Mediator function to pass control from the C++ only hologram to the data asset.
	UHologramOverrides* GetHologramOverride() const;

	// Sampling is when the player middle clicks on a building to auto start select it in the build gun.
	// This is normally only allowed by buildables with a recipe in the build gun menu.
	// Since the Solid standalone is the one listed in the build menu, it would be the only one sampleable.
	// This override makes it so all buildables inheriting from Build_TransmitterBase check if their sampleable by
	// seeing if the solid standalone is sampleable.
	//
	// WARNING: If this mod is expanded to include upgraded transmitters (not planned at time of writing)
	// they will need their own CanBeSampled override.
	virtual bool CanBeSampled_Implementation() const override;

private:
	UPROPERTY(EditDefaultsOnly)
	UFGFactoryConnectionComponent* inputConnection = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UHologramOverrides* HologramOverrides;
};
