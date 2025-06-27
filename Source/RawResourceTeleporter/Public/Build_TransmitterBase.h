// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"

#include "Build_TransmitterBase.generated.h"

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ABuild_TransmitterBase : public AFGBuildableFactory
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_TransmitterBase();

private:
	UPROPERTY(EditDefaultsOnly)
	UFGFactoryConnectionComponent* inputConnection = nullptr;
};
