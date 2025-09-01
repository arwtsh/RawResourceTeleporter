// 


#include "Build_ReceiverBase.h"

#include "UnrealNetwork.h"


// Sets default values
ABuild_ReceiverBase::ABuild_ReceiverBase() : Super()
{
	
}

void ABuild_ReceiverBase::SetResourceToTeleport(TSubclassOf<UFGItemDescriptor> Resource)
{
	
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
	UE_LOG(LogTemp, Warning, TEXT("GetAllowedTeleportableItems's default implementation was called."));
	return TSet<TSoftClassPtr<UFGItemDescriptor>>();
}
