// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildable.h"
#include "Build_TransmitterBase.generated.h"

class UTeleporterInterface;

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ABuild_TransmitterBase : public AFGBuildable
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_TransmitterBase();

private:
	UPROPERTY(EditDefaultsOnly)
	UFGFactoryConnectionComponent* inputConnection = nullptr;
};
