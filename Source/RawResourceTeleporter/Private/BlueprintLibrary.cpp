//

#include "public/BlueprintLibrary.h"

#include "FGItemDescriptor.h"
#include "RawResourceTeleporterWorldModule.h"
#include "RawResourceTeleporter.h"
#include "SessionSettingsManager.h"
#include "WorldModuleManager.h"

URawResourceTeleporterWorldModule* UBlueprintLibrary::GetRawResourceTeleporterWorldModule(UObject* WorldContextObject)
{
	UWorldModule* worldModule = WorldContextObject->GetWorld()->GetSubsystem<UWorldModuleManager>()->FindModule(ModReference);
	return Cast<URawResourceTeleporterWorldModule>(worldModule);
}

FString UBlueprintLibrary::SerializeTeleportableResources(const TSet<TSubclassOf<UFGItemDescriptor>> teleportableResources)
{
	TArray<FString> softClassArray;
	for (const TSubclassOf<UFGItemDescriptor> descriptorClass : teleportableResources)
	{
		if (IsValid(descriptorClass) && IsValid(descriptorClass.Get()))
		{
			FSoftClassPath path = FSoftClassPath(descriptorClass.Get());
			if (!path.IsNull())
			{
				softClassArray.Add(path.ToString());
			}
		}
	}
	FString result = FString::Join(softClassArray, TEXT(", "));
	return result;
}

TSet<TSubclassOf<UFGItemDescriptor>> UBlueprintLibrary::DeserializeTeleportableResources(const FString teleportableResources)
{
	TSet<TSubclassOf<UFGItemDescriptor>> result;

	TArray<FString> parsedArray;
	teleportableResources.ParseIntoArray(parsedArray, TEXT(", "));
	for (FString stringClass : parsedArray)
	{
		FSoftClassPath path = FSoftClassPath(stringClass);
		if (!path.IsNull())
		{
			TSubclassOf<UFGItemDescriptor> descriptor = path.TryLoadClass<UFGItemDescriptor>();
			if (IsValid(descriptor))
			{
				result.Add(descriptor);
			}
			else
			{
				UE_LOG(LogRawResourceTeleporter, Verbose, TEXT("Failed to parse TeleportableResource \"%s\" into an actual resource."), *path.ToString());
			}
		}
	}

	return result;
}

void UBlueprintLibrary::ForceSetStringOptionValue(const TScriptInterface<IFGOptionInterface> option, const FString cvar, const FString newValue)
{
	option->ForceSetOptionValue(cvar, newValue, option.GetObject());
}