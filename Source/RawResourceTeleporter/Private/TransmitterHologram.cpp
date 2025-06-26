// 

#include "TransmitterHologram.h"

ATransmitterHologram::ATransmitterHologram()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

bool ATransmitterHologram::TrySnapToActor(const FHitResult& hitResult)
{
	return Super::TrySnapToActor(hitResult);
	/*
	AActor* Actor = hitResult.GetActor();

	if (!IsValid(Actor))
	{
		// We moved away from our previous snapping, so clear our tracker
		Extractor = nullptr;
		return false;
	}

	if (Actor->IsA<AFGBuildableResourceExtractor>())
	{
		Extractor = Cast<AFGBuildableResourceExtractor>(Actor);
		// Here you can add your custom snapping logic.
		// We are using `SetActorLocationAndRotation` which is better for performance than setting single properties.

		
		
		SetActorLocationAndRotation(Actor->GetActorLocation(), Actor->GetActorRotation());
		// We snapped, so return true to disable following updates
		return true;
	}

	Extractor = nullptr;
	return false;*/
}
