#include "Hologram/HologramOverrides.h"

#include "ContentTagRegistry.h"
#include "GameplayTagContainer.h"


TOptional<TSubclassOf<UFGRecipe>> UHologramOverrides::GetHologramOverride(const UObject* WorldContext, UClass* hitResult) const
{
	if (!hitResult)
		return {};

	UContentTagRegistry* tagRegistry = UContentTagRegistry::Get(WorldContext);
	if (!tagRegistry)
		return {};
	
	FGameplayTagContainer tagContainer = tagRegistry->GetGameplayTagContainerFor(hitResult);

	//Short circuit early if we know there are no tags. As of 1.1, vanilla SF doesn't use tags.
	if (tagContainer.IsEmpty())
		return {};

	// See which, if any, tags the buildable has that matches the override data.
	// A buildable shouldn't have multiple tags for transmitter attachments but if it does, it only finds the first one.
	for (const TPair<FGameplayTag, TSubclassOf<UFGRecipe>>& Elem : OverrideData)
	{
		if (tagContainer.HasTagExact(Elem.Key))
		{
			return Elem.Value;
		}
	}

	return {};
}
