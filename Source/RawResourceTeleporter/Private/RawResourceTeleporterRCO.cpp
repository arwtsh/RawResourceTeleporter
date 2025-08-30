//

#include "RawResourceTeleporterRCO.h"
#include "Build_RecieverBase.h"


void URawResourceTeleporterRCO::Server_SetRecieverResourceToTeleport_Implementation(ABuild_RecieverBase* Reciever, TSubclassOf<UFGItemDescriptor> Resource)
{
	Reciever->ResourceToTeleport = Resource;
}

bool URawResourceTeleporterRCO::Server_SetRecieverResourceToTeleport_Validate(ABuild_RecieverBase* Reciever, TSubclassOf<UFGItemDescriptor> Resource)
{
	return Reciever->GetAllowedTeleportableItems().Contains(TSoftClassPtr<UFGItemDescriptor>(Resource));
}

void URawResourceTeleporterRCO::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
