// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"

#include "Build_RecieverBase.generated.h"

UCLASS()
class RAWRESOURCETELEPORTER_API ABuild_RecieverBase : public AFGBuildableFactory
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_RecieverBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
