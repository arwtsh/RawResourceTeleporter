// 

#include "Hologram/TransmitterBaseHologram.h"
#include "Hologram/TransmitterBuildModes.h"
#include "RawResourceTeleporter.h" //For logging
#include "Build_TransmitterBase.h"
#include "Hologram/HologramOverrides.h"


ATransmitterHologram::ATransmitterHologram() : AFGFactoryHologram()
{
	//Sets the default build mode.
	//If this wasn't set, then the starting build mode would be "Default" even if that wasn't added in GetSupportedBuildModes_Implementation
	mDefaultBuildMode = USolidTransmitterBuildMode::StaticClass();
}

TOptional<TSubclassOf<UFGRecipe>> ATransmitterHologram::ProcessHologramOverride(const FHitResult& hitResult) const
{
	AActor* hitActor = hitResult.GetActor();

	//Don't do anything if the player isn't looking at a different building,
	//since we already know the building the player is looking at doesn't have an override.
	//If lastLookedAt or hitActor is nullptr, that means the player wasn't looking at anything,
	//thus checking nullptr equality makes sense.
	if (lastLookedAt == hitActor)
		return {};
	else
		lastLookedAt = hitActor;

	UHologramOverrides* hologramOverrides = GetHologramOverrideData();

	if (!hologramOverrides)
		return{};
	
	return GetHologramOverride(hologramOverrides, hitActor);
}

TOptional<TSubclassOf<UFGRecipe>> ATransmitterHologram::GetHologramOverride(const UHologramOverrides* hologramOverrides, const AActor* hitActor) const
{
	// If the player is looking at something, pass control to the HologramOverride data asset assigned to the buildable
	// and if it has one of the tags associated with a Transmitter attachment, return the override.
	if (hitActor)
	{
		TOptional<TSubclassOf<UFGRecipe>> result = hologramOverrides->GetHologramOverride(this, hitActor->GetClass());

		return result;
	}

	//The player wasn't looking at something, so there wouldn't be any overrides.
	return {};
}

UHologramOverrides* ATransmitterHologram::GetHologramOverrideData() const
{
	ABuild_TransmitterBase* buildable = GetDefaultBuildable<ABuild_TransmitterBase>();

	if (!buildable)
		return nullptr;

	//If the override data asset wasn't assigned to the buildable, the GetHologramOverride function will log it and return nullptr.
	return buildable->GetHologramOverride();
}

void ATransmitterHologram::GetSupportedBuildModes_Implementation( TArray<TSubclassOf<UFGBuildGunModeDescriptor>>& out_buildmodes) const
{
	//Super::GetSupportedBuildModes_Implementation(out_buildmodes);
	
	out_buildmodes.Add(USolidTransmitterBuildMode::StaticClass());
	out_buildmodes.Add(UFluidTransmitterBuildMode::StaticClass());
}
