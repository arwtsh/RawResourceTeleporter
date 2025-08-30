// 


#include "Build_RecieverBase.h"

#include "UnrealNetwork.h"


// Sets default values
ABuild_RecieverBase::ABuild_RecieverBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SaveConfig();
}

void ABuild_RecieverBase::SetResourceToTeleport(TSubclassOf<UFGItemDescriptor> Resource)
{
	
}

void ABuild_RecieverBase::OnRep_ResourceToTeleport() const
{
	OnResourceToTeleportChanged.Broadcast(ResourceToTeleport);
}

void ABuild_RecieverBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABuild_RecieverBase, ResourceToTeleport);
}

TSet<TSoftClassPtr<UFGItemDescriptor>> ABuild_RecieverBase::GetAllowedTeleportableItems_Implementation() const
{
	UE_LOG(LogTemp, Warning, TEXT("GetAllowedTeleportableItems's default implementation was called."));
	return TSet<TSoftClassPtr<UFGItemDescriptor>>();
}
