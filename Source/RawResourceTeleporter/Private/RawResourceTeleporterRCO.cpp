//

#include "RawResourceTeleporterRCO.h"
#include "Build_ReceiverBase.h"
#include "UnrealNetwork.h"


void URawResourceTeleporterRCO::Server_SetReceiverResourceToTeleport_Implementation(ABuild_ReceiverBase* Receiver, TSubclassOf<UFGItemDescriptor> Resource)
{
	UE_LOG(LogTemp, Display, TEXT("Receiver SetReceiverResourceToTeleporter"));
	Receiver->ResourceToTeleport = Resource;
}

bool URawResourceTeleporterRCO::Server_SetReceiverResourceToTeleport_Validate(ABuild_ReceiverBase* Receiver, TSubclassOf<UFGItemDescriptor> Resource)
{
	UE_LOG(LogTemp, Display, TEXT("Receiver SetReceiverResourceToTeleport_Validate"));

	return Receiver->GetAllowedTeleportableItems().Contains(TSoftClassPtr<UFGItemDescriptor>(Resource));
}

void URawResourceTeleporterRCO::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URawResourceTeleporterRCO, bDummy);
}
