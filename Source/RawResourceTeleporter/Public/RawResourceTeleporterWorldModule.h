// 

#pragma once

#include "CoreMinimal.h"
#include "GameWorldModule.h"
#include "RawResourceTeleporterWorldModule.generated.h"

class UFGItemDescriptor;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTeleportableResourcesUpdated, TSet<TSoftClassPtr<UFGItemDescriptor>>&, newTeleportableResources);

/**
 * 
 */
UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API URawResourceTeleporterWorldModule : public UGameWorldModule
{
	GENERATED_BODY()

private:
	const FString CONFIG_ID = TEXT("RawResourceTeleporter.TeleportableResourcesWhitelist");

public:

	UPROPERTY(BlueprintReadWrite)
	TSet<TSoftClassPtr<UFGItemDescriptor>> solidTeleportableResources;

	UPROPERTY(BlueprintReadWrite)
	TSet<TSoftClassPtr<UFGItemDescriptor>> fluidTeleportableResources;

	UPROPERTY(BlueprintAssignable)
	FOnTeleportableResourcesUpdated OnSolidTeleportableResourcesUpdated;
	
	UPROPERTY(BlueprintAssignable)
	FOnTeleportableResourcesUpdated OnFluidTeleportableResourcesUpdated;
	
private:
	UFUNCTION()
	void UpdateTeleportableResources(const FString& updatedCvar);

	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;
};
