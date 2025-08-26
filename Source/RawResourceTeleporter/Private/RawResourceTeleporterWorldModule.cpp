// 

#include "RawResourceTeleporterWorldModule.h"

#include "FGItemDescriptor.h"
#include "SessionSettingsManager.h"
#include "RawResourceTeleporter.h"
#include "BlueprintLibrary.h"

void URawResourceTeleporterWorldModule::UpdateTeleportableResources(const FString& updatedCvar)
{
	if (!CONFIG_ID.Equals(updatedCvar))
	{
		UE_LOG(LogRawResourceTeleporter, Verbose, TEXT("Failed to update cached list of teleportable resources. The updatedCvar \"%s\" is not equal to the constant \"%s\""), *updatedCvar, *CONFIG_ID);
		return;
	}

	const USessionSettingsManager* SessionSettings = GetWorld()->GetSubsystem<USessionSettingsManager>();

	const FString rawValue = SessionSettings->GetOptionValueTyped<FString>(CONFIG_ID);
	TSet<TSubclassOf<UFGItemDescriptor>> teleportableResources = UBlueprintLibrary::DeserializeTeleportableResources(rawValue);

	solidTeleportableResources.Empty();
	fluidTeleportableResources.Empty();

	//Sort the allow list into solids and fluids and convert to soft class so they aren't required to be loaded.
	for (TSubclassOf<UFGItemDescriptor> teleportableResource : teleportableResources)
	{
		if (UFGItemDescriptor::CanItemBePickedup(teleportableResource))
		{
			solidTeleportableResources.Add(TSoftClassPtr<UFGItemDescriptor>(teleportableResource));
		}
		else
		{
			fluidTeleportableResources.Add(TSoftClassPtr<UFGItemDescriptor>(teleportableResource));
		}
	}

	OnSolidTeleportableResourcesUpdated.Broadcast(solidTeleportableResources);
	OnFluidTeleportableResourcesUpdated.Broadcast(fluidTeleportableResources);
}

void URawResourceTeleporterWorldModule::DispatchLifecycleEvent(ELifecyclePhase Phase)
{
	Super::DispatchLifecycleEvent(Phase);
	
	if (Phase != ELifecyclePhase::POST_INITIALIZATION)
		return;

    USessionSettingsManager* SessionSettings = GetWorld()->GetSubsystem<USessionSettingsManager>();

	//Every time the setting is modified, change the cached TSet.
	FOptionUpdated optionDelegate;
	optionDelegate.BindUFunction(this, FName("UpdateTeleportableResources"));
	SessionSettings->SubscribeToDynamicOptionUpdate(CONFIG_ID, optionDelegate);
	
	//Populate the initial value of teleportableResources from the saved data or default
	UpdateTeleportableResources(CONFIG_ID);
}
