// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"

#include "Build_ReceiverBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceToTeleportChanged, TSubclassOf<UFGItemDescriptor>, Resource);

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ABuild_ReceiverBase : public AFGBuildableFactory
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_ReceiverBase();

	void SetResourceToTeleport(TSubclassOf<UFGItemDescriptor> Resource);

public:
	UPROPERTY(BlueprintAssignable)
	FOnResourceToTeleportChanged OnResourceToTeleportChanged;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing="OnRep_ResourceToTeleport", SaveGame)
	TSubclassOf<UFGItemDescriptor> ResourceToTeleport;
	
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSet<TSoftClassPtr<UFGItemDescriptor>> GetAllowedTeleportableItems() const;

	UFUNCTION()
	void OnRep_ResourceToTeleport() const;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
