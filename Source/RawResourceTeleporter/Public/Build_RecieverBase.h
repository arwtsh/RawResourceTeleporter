// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"

#include "Build_RecieverBase.generated.h"

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ABuild_RecieverBase : public AFGBuildableFactory
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_RecieverBase();
};
