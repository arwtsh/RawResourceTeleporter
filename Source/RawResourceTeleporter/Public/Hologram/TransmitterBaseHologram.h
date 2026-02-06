// 

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGFactoryHologram.h"
#include "TransmitterBaseHologram.generated.h"

class UHologramOverrides;

UCLASS(ABSTRACT)
class RAWRESOURCETELEPORTER_API ATransmitterHologram : public AFGFactoryHologram
{
	GENERATED_BODY()

public:
	// Sets default values
	ATransmitterHologram();
	
	// Adds build modes to this hologram.
	// Build modes are used to switch between solid and fluid standalone transmitters.
	// Because the attachments can override to a standalone when the realize they are not focused on a supported buildable,
	// they also need the build modes so they know which to override to, even though the building itself doesn't use the build modes.
	virtual void GetSupportedBuildModes_Implementation(TArray<TSubclassOf<UFGBuildGunModeDescriptor>>& out_buildmodes) const override;

	// This is called every frame by the build gun.
	// If it returns a recipe for a different object, the build gun completely deletes this current hologram and makes a new one.
	virtual TOptional<TSubclassOf<UFGRecipe>> ProcessHologramOverride(const FHitResult& hitResult) const override;

protected:
	// This is called by ProcessHologramOverride.
	// After it determines that it should check for an override
	// (because the player might not be looking at something new or the hologram override data wasn't assigned to the buildable)
	// it passes control to this function.
	// This determines which, if any, override should be used.
	virtual TOptional<TSubclassOf<UFGRecipe>> GetHologramOverride(const UHologramOverrides* hologramOverrides, const AActor* hitActor) const;

	// Helper function that retrieves the HologramOverride data asset from the buildable.
	// The data asset is on the blueprint level, so only blueprints can access it. Since holograms shouldn't be blueprinted,
	// It must be placed on the buildable.
	UHologramOverrides* GetHologramOverrideData() const;

private:
	// Used so ProcessHologramOverride doesn't execute every frame.
	// Instead, it runs only when the player is looking at a new actor.
	UPROPERTY()
	mutable AActor* lastLookedAt;
};