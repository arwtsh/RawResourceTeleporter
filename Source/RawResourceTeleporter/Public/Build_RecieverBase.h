// 

#pragma once

#include "CoreMinimal.h"
#include "FGBuildableFactory.h"

#include "Build_RecieverBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnResourceToTeleportChanged, TSubclassOf<UFGItemDescriptor>, Resource);

UCLASS(Abstract)
class RAWRESOURCETELEPORTER_API ABuild_RecieverBase : public AFGBuildableFactory
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuild_RecieverBase();

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
