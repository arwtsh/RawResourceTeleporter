// 

#pragma once

#include "CoreMinimal.h"
#include "FGRemoteCallObject.h"
#include "RawResourceTeleporterRCO.generated.h"

class ABuild_ReceiverBase;
class UFGItemDescriptor;

/**
 * 
 */
UCLASS()
class RAWRESOURCETELEPORTER_API URawResourceTeleporterRCO : public UFGRemoteCallObject
{
	GENERATED_BODY()

private:
	//This doesn't do anything. If a RCO doesn't have a replicated property it won't work properly.
	UPROPERTY(Replicated)
	bool bDummy = true;
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	static URawResourceTeleporterRCO* Get(const UObject* WorldContext);

public:
	UFUNCTION(Server, Reliable)
	void Server_SetReceiverResourceToTeleport(ABuild_ReceiverBase* Receiver, TSubclassOf<UFGItemDescriptor> Resource);
};
