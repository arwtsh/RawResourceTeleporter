// 


#include "Build_ReceiverBase.h"
#include "RawResourceTeleporter.h"
#include "RawResourceTeleporterRCO.h"
#include "UnrealNetwork.h"


// Sets default values
ABuild_ReceiverBase::ABuild_ReceiverBase() : Super()
{
	
}

void ABuild_ReceiverBase::SetResourceToTeleport(TSubclassOf<UFGItemDescriptor> Resource)
{
	if (!Resource)
		return;
	
	if (HasAuthority())
	{
		//Executed on server

		if (Resource == ResourceToTeleport || !GetAllowedTeleportableItems().Contains(TSoftClassPtr<UFGItemDescriptor>(Resource)))
		{
			return;
		}

		ResourceToTeleport = Resource;
		OnRep_ResourceToTeleport(); //This gets automatically called on clients, but it needs to be manually called on the server.
	}
	else
	{
		//Executed on client

		//Go tell the server to run this code.
		if (URawResourceTeleporterRCO* RCO = URawResourceTeleporterRCO::Get(this))
		{
			RCO->Server_SetReceiverResourceToTeleport(this, Resource);
		}
	}
}

void ABuild_ReceiverBase::OnRep_ResourceToTeleport() const
{
	OnResourceToTeleportChanged.Broadcast(ResourceToTeleport);
}

void ABuild_ReceiverBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABuild_ReceiverBase, ResourceToTeleport);
}

TSet<TSoftClassPtr<UFGItemDescriptor>> ABuild_ReceiverBase::GetAllowedTeleportableItems_Implementation() const
{
	UE_LOG(LogRawResourceTeleporter, Warning, TEXT("GetAllowedTeleportableItems's default implementation was called."));
	return TSet<TSoftClassPtr<UFGItemDescriptor>>();
}
