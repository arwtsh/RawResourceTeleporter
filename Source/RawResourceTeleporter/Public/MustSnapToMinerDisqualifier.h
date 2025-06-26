// 

#pragma once

#include "CoreMinimal.h"
#include "FGConstructDisqualifier.h"
#include "MustSnapToMinerDisqualifier.generated.h"

#define LOCTEXT_NAMESPACE "RawResourceTeleporter"

/**
 * 
 */
UCLASS()
class RAWRESOURCETELEPORTER_API UMustSnapToMinerDisqualifier : public UFGConstructDisqualifier
{
	GENERATED_BODY()

	UMustSnapToMinerDisqualifier()
	{
		mDisqfualifyingText = LOCTEXT("UMustSnapToMinerDisqualifier", "Resource Transmitter must connect to a Miner!");
	}
};

#undef LOCTEXT_NAMESPACE