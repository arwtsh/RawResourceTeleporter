// 

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "BlueprintLibrary.generated.h"

class URawResourceTeleporterWorldModule;
class UFGItemDescriptor;

/**
 * 
 */
UCLASS()
class RAWRESOURCETELEPORTER_API UBlueprintLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	inline static const FName ModReference = FName(TEXT("RawResourceTeleporter"));
public:
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"))
	static URawResourceTeleporterWorldModule* GetRawResourceTeleporterWorldModule(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintCallable)
	static FString SerializeTeleportableResources(const TSet<TSubclassOf<UFGItemDescriptor>> teleportableResources);

	UFUNCTION(BlueprintCallable)
	static TSet<TSubclassOf<UFGItemDescriptor>> DeserializeTeleportableResources(const FString teleportableResources);

	UFUNCTION(BlueprintCallable)
	static void ForceSetStringOptionValue(TScriptInterface<IFGOptionInterface> option, const FString cvar, const FString newValue);

};
