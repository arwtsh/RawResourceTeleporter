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

public:
	//This doesn't do anything. If a RCO doesn't have a replicated property it won't work properly.
	UPROPERTY(Replicated)
	bool bDummy = true;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	UFUNCTION(Server, WithValidation, Reliable, BlueprintCallable)
	void Server_SetReceiverResourceToTeleport(ABuild_ReceiverBase* Receiver, TSubclassOf<UFGItemDescriptor> Resource);
};
