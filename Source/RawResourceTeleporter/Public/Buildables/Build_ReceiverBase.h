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

private:
	void ResourceToTeleportChanged();

	// This is a timer, saved from 0%-100% (0-1.0f)
	// When it is >1, then the receiver will teleport items.
	UPROPERTY( SaveGame, Meta = ( NoAutoJson = true, FGReplicated ) )
	float CurrentCycleProgress;

	//The raw time it takes for this receiver to teleport a group of items.
	//This does not factor in speed modifiers such as over/under clocking.
	UPROPERTY(EditDefaultsOnly)
	float CycleLengthSeconds;
	float cachedCycleLength; //Calculated cycle length from overclocking, cached to improve performance

	bool cachedHasOutputSpace;

	void CacheHasOutputSpace();

	void CalculateCycleLength();
public:
	// Sets default values for this actor's properties
	ABuild_ReceiverBase();
	virtual void BeginPlay() override;

public:
	UPROPERTY(BlueprintAssignable)
	FOnResourceToTeleportChanged OnResourceToTeleportChanged;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_ResourceToTeleport, SaveGame)
	TSubclassOf<UFGItemDescriptor> ResourceToTeleport;
	
	UPROPERTY(BlueprintReadOnly) //Remove SaveGame maybe?
	class UFGInventoryComponent* OutputInventory;

	UPROPERTY(EditDefaultsOnly)
	float EmergencyCrateOffset;

	//The number of items to teleport per cycle.
	//This is useful for reducing the length of a cycle, instead of it teleporting 1 item 4800 times a minute (4 mk6 belt outputs), it could teleport 4 items 1200 times a minute
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int ItemsPerCycle;

protected:
	virtual void Factory_TickProducing( float dt ) override;
	virtual bool CanProduce_Implementation() const override;
	virtual bool IsConfigured() const override;

	//This divided by 60 is per minute production
	virtual float GetProductionCycleTime() const override;
	virtual void OnRep_CurrentPotential() override;
	virtual float GetDefaultProductionCycleTime() const override;
	virtual float CalcProductionCycleTimeForPotential(float potential) const override;

	UFUNCTION()
	void OnOutputInventoryChanged( TSubclassOf< UFGItemDescriptor > itemClass, const int32 numAdded, UFGInventoryComponent* sourceInventory = nullptr );

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TSet<TSoftClassPtr<UFGItemDescriptor>> GetAllowedTeleportableItems() const;

	UFUNCTION()
	void OnRep_ResourceToTeleport();
	
	void SetResourceToTeleport(TSubclassOf<UFGItemDescriptor> Resource);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	
	virtual float GetProductivity() const override;
	virtual float GetProductionProgress() const override;

	//Must be called on the main thread if Player is null.
	void EjectInventory(AFGCharacterPlayer* Player) const;
};
