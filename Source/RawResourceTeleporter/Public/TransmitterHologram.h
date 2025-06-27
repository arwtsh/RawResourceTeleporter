// 

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGBuildableHologram.h"
#include "Buildables/FGBuildableResourceExtractor.h"
#include "TransmitterHologram.generated.h"

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ATransmitterHologram : public AFGBuildableHologram
{
	GENERATED_BODY()

protected:
	/** name used to match types of extractors for compatibility */
	UPROPERTY( EditDefaultsOnly )
	FName extractorTypeName = "";

private:
	UFGFactoryConnectionComponent* cachedExtractorConnection = nullptr;
	UFGFactoryConnectionComponent* cachedTransmitterConnection = nullptr;
		
public:
	// Sets default values for this actor's properties
	ATransmitterHologram();

	virtual void BeginPlay() override;
	
	bool TrySnapToActor(const FHitResult& hitResult) override;

	void CheckValidPlacement() override;

	bool IsValidHitResult(const FHitResult& hitResult) const override;

	bool ShouldActorBeConsideredForGuidelines(AActor* actor) const override;
	
private:
	void SnapToConnection(const UFGFactoryConnectionComponent* connection);
};
