// 

#include "RawResourceTeleporterWorldModule.h"

#include "FGItemDescriptor.h"
#include "SessionSettingsManager.h"
#include "RawResourceTeleporter.h"
#include "BlueprintLibrary.h"

TSet<TSubclassOf<UFGItemDescriptor>> URawResourceTeleporterWorldModule::teleportableResources = TSet<TSubclassOf<UFGItemDescriptor>>();

void URawResourceTeleporterWorldModule::UpdateTeleportableResources(const FString& updatedCvar) const
{
	const FString strId = TEXT("RawResourceTeleporter.TeleportableResourcesWhitelist");

	//I don't think updatedCvar can be anything else because the SubscribeToDynamicOptionUpdate passes this value in.
	if (!strId.Equals(updatedCvar))
	{
		UE_LOG(LogRawResourceTeleporter, Verbose, TEXT("Failed to update cached list of teleportable resources. The updatedCvar \"%s\" is not equal to the constant \"%s\""), *updatedCvar, *strId);
		return;
	}
	
	const USessionSettingsManager* SessionSettings = GetWorld()->GetSubsystem<USessionSettingsManager>();
	const FString rawValue = SessionSettings->GetOptionValueTyped<FString>(strId);
	teleportableResources = UBlueprintLibrary::DeserializeTeleportableResources(rawValue);
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
	SessionSettings->SubscribeToDynamicOptionUpdate(TEXT("RawResourceTeleporter.TeleportableResourcesWhitelist"), optionDelegate);
	
	//Populate the initial value of teleportableResources from the saved data or default
	UpdateTeleportableResources(TEXT("RawResourceTeleporter.TeleportableResourcesWhitelist"));
}

TSet<TSubclassOf<UFGItemDescriptor>> URawResourceTeleporterWorldModule::GetTeleportableResources()
{
	return teleportableResources;
}
