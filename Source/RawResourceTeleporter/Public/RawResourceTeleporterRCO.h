// 

#pragma once

#include "CoreMinimal.h"
#include "FGRemoteCallObject.h"
#include "RawResourceTeleporterRCO.generated.h"

class ABuild_RecieverBase;
class UFGItemDescriptor;

/**
 * 
 */
UCLASS()
class RAWRESOURCETELEPORTER_API URawResourceTeleporterRCO : public UFGRemoteCallObject
{
	GENERATED_BODY()

public:
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void Server_SetRecieverResourceToTeleport(ABuild_RecieverBase* Reciever, TSubclassOf<UFGItemDescriptor> Resource);

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
};
