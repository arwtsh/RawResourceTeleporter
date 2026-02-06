#pragma once
#include "CoreMinimal.h"
#include "TransmitterBaseHologram.h"

#include "Buildables/FGBuildableResourceExtractor.h"

#include "TransmitterAttachmentHologram.generated.h"


class UHologramOverrides;

UCLASS()
class RAWRESOURCETELEPORTER_API ATransmitterAttachmentHologram : public ATransmitterHologram
{
	GENERATED_BODY()

public:
	//The attachment hologram needs to have additional logic to override to a standalone.
	virtual TOptional<TSubclassOf<UFGRecipe>> GetHologramOverride(const UHologramOverrides* hologramOverrides, const AActor* hitActor) const override;

	// The hologram will always be hovered over a building. If it is not, then it will become the Standalone hologram
	// which itself handles valid hit result. So this can always return true because it is never used.
	virtual bool IsValidHitResult(const FHitResult& hitResult) const override { return true;}


	
protected:
	/** name used to match types of extractors for compatibility */
	UPROPERTY( EditDefaultsOnly )
	FName extractorTypeName = "";

private:
	UFGFactoryConnectionComponent* cachedExtractorConnection = nullptr;
	UFGFactoryConnectionComponent* cachedTransmitterConnection = nullptr;	
		
public:


	virtual void BeginPlay() override;
	
	bool TrySnapToActor(const FHitResult& hitResult) override;

	void CheckValidPlacement() override;
	
	bool ShouldActorBeConsideredForGuidelines(AActor* actor) const override;
	
private:
	void SnapToConnection(const UFGFactoryConnectionComponent* connection);
};
