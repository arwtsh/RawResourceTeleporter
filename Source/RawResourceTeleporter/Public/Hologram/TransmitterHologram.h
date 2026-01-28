// 

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGBuildableHologram.h"
#include "Buildables/FGBuildableResourceExtractor.h"
#include "TransmitterHologram.generated.h"

UENUM()
enum class ETransmitterType : uint8
{
	Solid,
	Fluid,
	Attachment
};

UCLASS()
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

	ETransmitterType transmitterType = ETransmitterType::Solid;
		
public:
	// Sets default values for this actor's properties
	ATransmitterHologram();

	virtual void BeginPlay() override;
	
	bool TrySnapToActor(const FHitResult& hitResult) override;

	void CheckValidPlacement() override;

	bool IsValidHitResult(const FHitResult& hitResult) const override;

	bool ShouldActorBeConsideredForGuidelines(AActor* actor) const override;

	// This is called every frame.
	// If it returns a recipe for a different object, it completely deletes this current hologram and makes a new one.
	virtual TOptional<TSubclassOf<UFGRecipe>> ProcessHologramOverride(const FHitResult& hitResult) const override;
	
private:
	void SnapToConnection(const UFGFactoryConnectionComponent* connection);
};
