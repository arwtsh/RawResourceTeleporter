#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataAsset.h"
#include "HologramOverrides.generated.h"

struct FGameplayTag;

UCLASS(BlueprintType)
class RAWRESOURCETELEPORTER_API UHologramOverrides : public UDataAsset
{
	GENERATED_BODY()

public:
	TOptional<TSubclassOf<UFGRecipe>> GetHologramOverride( const UObject* WorldContext, UClass* hitResult ) const;

	FORCEINLINE TSubclassOf<UFGRecipe> GetSolidStandaloneTransmitter() const { return SolidStandalone; }
	FORCEINLINE TSubclassOf<UFGRecipe> GetFluidStandaloneTransmitter() const { return FluidStandalone; }
 
private:
	// Links a buildable, like Miner or Water Extractor, to a variation of the Transmitter that will attach to the buildable.
	// Uses gameplay tags to identify which buildable should have which transmitter.
	//This is a hard reference because that's what vanilla does, even if I would prefer a soft class ptr.
	//This actually links to a recipe which links to the transmitter, but that's how hologram overrides work.
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, TSubclassOf<UFGRecipe>> OverrideData;

	//If the player isn't looking at a buildable with an associated tag, switch to one of these versions of standalone.
	//Solid vs fluid depends on the hologram's build mode.
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGRecipe> SolidStandalone;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UFGRecipe> FluidStandalone;
};
