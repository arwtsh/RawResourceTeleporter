// 

#pragma once

#include "CoreMinimal.h"
#include "GameWorldModule.h"
#include "RawResourceTeleporterWorldModule.generated.h"

class UFGItemDescriptor;

/**
 * 
 */
UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API URawResourceTeleporterWorldModule : public UGameWorldModule
{
	GENERATED_BODY()
	
private:
	static TSet<TSubclassOf<UFGItemDescriptor>> teleportableResources;

private:
	UFUNCTION()
	void UpdateTeleportableResources(const FString& updatedCvar) const;

	virtual void DispatchLifecycleEvent(ELifecyclePhase Phase) override;
	
public:
	static TSet<TSubclassOf<UFGItemDescriptor>> GetTeleportableResources();
};
