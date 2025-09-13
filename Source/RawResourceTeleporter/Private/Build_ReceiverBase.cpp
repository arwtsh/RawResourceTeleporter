// 


#include "Build_ReceiverBase.h"

#include "FGCharacterPlayer.h"
#include "FGItemPickup_Spawnable.h"
#include "RawResourceTeleporter.h"
#include "RawResourceTeleporterWorldModule.h"
#include "UnrealNetwork.h"


// Sets default values
ABuild_ReceiverBase::ABuild_ReceiverBase() : Super()
{
	OutputInventory = CreateDefaultSubobject<UFGInventoryComponent>(TEXT("OutputInventory"));
}

void ABuild_ReceiverBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority())
	{
		OutputInventory->SetReplicationRelevancyOwner(this);
	}
	
	ResourceToTeleportChanged();

	OutputInventory->OnItemAddedDelegate.AddUniqueDynamic(this, &ThisClass::ABuild_ReceiverBase::OnOutputInventoryChanged);
	OutputInventory->OnItemRemovedDelegate.AddUniqueDynamic(this, &ThisClass::ABuild_ReceiverBase::OnOutputInventoryChanged);

	CacheHasOutputSpace();
	CalculateCycleLength();
}

void ABuild_ReceiverBase::ResourceToTeleportChanged()
{
	if (ResourceToTeleport)
	{
		OutputInventory->SetAllowedItemOnIndex(0, ResourceToTeleport);
		OutputInventory->SetLocked(false);
	}
	else
	{
		//Nothing can get dragged into the inventory.
		//OutputInventory->SetAllowedItemOnIndex(0, nullptr); doesn't work, it allows everything.
		OutputInventory->SetLocked(true);
	}
	
	CacheHasOutputSpace();
}

void ABuild_ReceiverBase::SetResourceToTeleport(TSubclassOf<UFGItemDescriptor> Resource)
{
	if (!Resource || !HasAuthority())
		return;
	
	if (Resource == ResourceToTeleport || !GetAllowedTeleportableItems().Contains(TSoftClassPtr<UFGItemDescriptor>(Resource)))
	{
		return;
	}

	ResourceToTeleport = Resource;
	OnRep_ResourceToTeleport(); //This gets automatically called on clients, but it needs to be manually called on the server.
}

bool ABuild_ReceiverBase::CanProduce_Implementation() const
{
	return cachedHasOutputSpace && Super::CanProduce_Implementation();
}

bool ABuild_ReceiverBase::IsConfigured() const
{
	return IsValid(ResourceToTeleport);
}

void ABuild_ReceiverBase::OnRep_CurrentPotential()
{
	Super::OnRep_CurrentPotential();
	CalculateCycleLength();
}

float ABuild_ReceiverBase::GetDefaultProductionCycleTime() const
{
	return CycleLengthSeconds;
}

float ABuild_ReceiverBase::CalcProductionCycleTimeForPotential(float potential) const
{
	return GetDefaultProductionCycleTime() / potential;
}

void ABuild_ReceiverBase::OnOutputInventoryChanged(TSubclassOf<UFGItemDescriptor> itemClass, const int32 numAdded, UFGInventoryComponent* sourceInventory)
{
	CacheHasOutputSpace();
}

void ABuild_ReceiverBase::CacheHasOutputSpace()
{
	//The receiver will try to top off, even if the ItemsPerCycle is more than 1.
	cachedHasOutputSpace = OutputInventory->HasEnoughSpaceForStack(FInventoryStack(1, ResourceToTeleport));
}

void ABuild_ReceiverBase::CalculateCycleLength()
{
	cachedCycleLength = CalcProductionCycleTimeForPotential(GetCurrentPotential());
}

void ABuild_ReceiverBase::Factory_TickProducing(float dt)
{
	Super::Factory_TickProducing(dt);

	//UE_LOG(LogRawResourceTeleporter, Warning, TEXT("TickProducing"));

	if (!HasAuthority())
		return;
	
	//if (!OutputInventory)
	//	return;

	CurrentCycleProgress = dt / GetProductionCycleTime() + CurrentCycleProgress;
	while (CurrentCycleProgress >= 1.0)
	{
		FInventoryStack inventory;
		OutputInventory->GetStackFromIndex(0, inventory);
		
		int SlotSize;
		SlotSize = OutputInventory->GetSlotSize(0, ResourceToTeleport);

		/* Experiment with setting slot size.
		switch (UFGItemDescriptor::GetForm(ResourceToTeleport))
		{
		case EResourceForm::RF_LIQUID:
		case EResourceForm::RF_GAS:
			SlotSize = OutputInventory->GetSlotSize(0, ResourceToTeleport);
			break;
		default:
			SlotSize = UFGItemDescriptor::GetStackSize(ResourceToTeleport);
		}
		*/

		//How many items to teleport. If there is less room in OutputInventory than ItemsPerCycle, fill up the remainding.
		//Since this production ticked once, it must teleport at least one item.
		//If the inventory is truly full, then CanProduce should have returned false.
		//Thus, it's assumed that the items are mismatched and should be ejected.
		int itemsThisCycle = FMath::Max(FMath::Min(ItemsPerCycle, SlotSize - inventory.NumItems), 1); 

		for (int i = 0; i < itemsThisCycle; i++)
		{
			if (URawResourceTeleporterWorldModule::TryTeleportResource(ResourceToTeleport))
			{
				if (!OutputInventory->AddItem(FInventoryItem(ResourceToTeleport)))
				{
					UE_LOG(LogRawResourceTeleporter, Error, TEXT("Failed to teleport resource."))
				}
			}
			else
			{
				//Temp log
				UE_LOG(LogRawResourceTeleporter, Warning, TEXT("Failed to get item from Teleporter"));
				//If it was unable to teleport a resource, all transmitters are empty.
				break;
			}
		}

		//Also have some sort of math for fluid only
		
		Factory_ProductionCycleCompleted(0); //The parameter doesn't matter (I think)

		CurrentCycleProgress -= 1.0;
	}
}

void ABuild_ReceiverBase::OnRep_ResourceToTeleport()
{
	OnResourceToTeleportChanged.Broadcast(ResourceToTeleport);
	ResourceToTeleportChanged();
}

void ABuild_ReceiverBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// Here we list the variables we want to replicate
	DOREPLIFETIME(ABuild_ReceiverBase, ResourceToTeleport);
}

float ABuild_ReceiverBase::GetProductionCycleTime() const
{
	//60/this = per minute
	return cachedCycleLength;
}

float ABuild_ReceiverBase::GetProductivity() const
{
	//return 0.1333;
	return Super::GetProductivity();
}

float ABuild_ReceiverBase::GetProductionProgress() const
{
	return CurrentCycleProgress;
}

void ABuild_ReceiverBase::EjectInventory(AFGCharacterPlayer* Player) const
{
	if (Player)
	{
		MoveOrDropInventory(OutputInventory, Player);
	}

	if (OutputInventory->IsEmpty())
		return;

	//If it didn't get emptied (either from the Player param being null or MoveOrDropInventory failed) drop it as a crate.
	FInventoryStack inventory;
	OutputInventory->GetStackFromIndex(0, inventory);
	FVector location;
	FRotator rotation;
	AFGItemPickup_Spawnable::FindGroundLocationInfrontOfActor(this, EmergencyCrateOffset, inventory, location, rotation);

	AFGCrate* spawnedCrate = nullptr;
	AFGItemPickup_Spawnable::SpawnInventoryCrate(GetWorld(), {inventory}, location, TArray<AActor*>(), spawnedCrate, EFGCrateType::CT_DismantleCrate);

	OutputInventory->Empty();
}

TSet<TSoftClassPtr<UFGItemDescriptor>> ABuild_ReceiverBase::GetAllowedTeleportableItems_Implementation() const
{
	UE_LOG(LogRawResourceTeleporter, Warning, TEXT("GetAllowedTeleportableItems's default implementation was called."));
	return TSet<TSoftClassPtr<UFGItemDescriptor>>();
}
