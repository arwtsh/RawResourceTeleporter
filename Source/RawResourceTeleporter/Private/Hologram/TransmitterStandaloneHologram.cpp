#include "Hologram/TransmitterStandaloneHologram.h"
#include "ContentTagRegistry.h"
#include "GameplayTagContainer.h"
#include "Hologram/HologramOverrides.h"
#include "RawResourceTeleporter.h" //For logging


bool ATransmitterStandaloneHologram::IsValidHitActor(AActor* hitActor) const
{
	//If it isn't looking at an actor, it's obviously false.
	if (!hitActor)
		return false;

	//If the hitActor is the same from the last time this function was called,
	//Then we can just re-return the previous value.
	//This can happen after we null check hitActor, so lastActorIsValidHit stores the previous real actor we checked against,
	//which still works since a null hitActor is always false.
	if (hitActor == lastActorIsValidHit)
		return lastResultIsValidHit;
	else
		lastActorIsValidHit = hitActor;
	
	//Check if it's a normal valid actor first, like a foundation, since those would be more common.
	if (Super::IsValidHitActor(hitActor))
		return lastResultIsValidHit = true;
	

	UHologramOverrides* hologramOverrides = GetHologramOverrideData();
	
	if (!hologramOverrides)
		return lastResultIsValidHit = false;

	UE_LOG(LogRawResourceTeleporter, Verbose, TEXT("IsValidHitActor called for Standalone."));

	//If there is a valid override for the actor, then it must be a valid hit result.
	TOptional<TSubclassOf<UFGRecipe>> result = hologramOverrides->GetHologramOverride(this, hitActor->GetClass());
	return lastResultIsValidHit = result.IsSet();	
}
