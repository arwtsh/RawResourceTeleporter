// 

#pragma once

#include "CoreMinimal.h"
#include "Hologram/FGConveyorAttachmentHologram.h"
#include "Buildables/FGBuildableResourceExtractor.h"
#include "TransmitterHologram.generated.h"

UCLASS()
class RAWRESOURCETELEPORTER_API ATransmitterHologram : public AFGConveyorAttachmentHologram
{
	GENERATED_BODY()

private:
	AFGBuildableResourceExtractor* Extractor = nullptr;
	
public:
	// Sets default values for this actor's properties
	ATransmitterHologram();

	bool TrySnapToActor(const FHitResult& hitResult) override;
};
