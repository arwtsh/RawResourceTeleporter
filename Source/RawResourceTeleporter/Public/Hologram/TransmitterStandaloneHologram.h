#pragma once
#include "CoreMinimal.h"
#include "TransmitterBaseHologram.h"
#include "TransmitterStandaloneHologram.generated.h"

UCLASS()
class RAWRESOURCETELEPORTER_API ATransmitterStandaloneHologram : public ATransmitterHologram
{
	GENERATED_BODY()

public:
	// The base functionality is holograms disappear when hovering over a factory building.
	// This should stay the same for normal buildings, but for buildings that can have an attachment transmitter,
	// it should return true, otherwise the build gun won't call ProcessHologramOverride to switch to the attachment transmitter.
	virtual bool IsValidHitActor( AActor* hitActor ) const override;

private:
	// Used so IsValidHitActor doesn't execute every frame.
	// Instead, it runs only when the player is looking at a new actor.
	UPROPERTY()
	mutable AActor* lastActorIsValidHit;
	UPROPERTY()
	mutable bool lastResultIsValidHit;
};
