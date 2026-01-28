// 

#include "TransmitterHologram.h"

#include "RawResourceTeleporter.h"
#include "Build_TransmitterBase.h"
#include "FGFactoryConnectionComponent.h"
#include "MustSnapToMinerDisqualifier.h"
#include "Hologram/HologramOverrides.h"

ATransmitterHologram::ATransmitterHologram()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ATransmitterHologram::BeginPlay()
{
	//To-do: delete this
	UE_LOG(LogRawResourceTeleporter, Error, TEXT("New hologram"));

	Super::BeginPlay();

	// The hologram should only have 1 input connection. It will crash if there are none.
	cachedTransmitterConnection = GetCachedFactoryConnectionComponents()[0];

	// Can be placed hovering over a surface as long as it's attached to the extractor.
	SetNeedsValidFloor(false);
}

bool ATransmitterHologram::TrySnapToActor(const FHitResult& hitResult)
{
	const AActor* Actor = hitResult.GetActor();

	if (!(IsValid(Actor) && Actor->IsA<AFGBuildableResourceExtractor>()))
	{
		return false;
	}

	const AFGBuildableResourceExtractor* extractor = Cast<AFGBuildableResourceExtractor>(Actor);

	if (!IsValid(extractor))
	{
		return false;
	}

	// Since all extractors, miners, water pumps, oil, etc. all use the same base class, a FName seperates the type.
	if (!extractor->GetExtractorTypeName().IsEqual(extractorTypeName))
	{
		return false;
	}

	// Finds which connection the transmitter should snap to.
	// Modded extractors might have multiple output connectors, so it would be best to 
	TSet<UActorComponent*> components = extractor->GetComponents();
	UFGFactoryConnectionComponent* closestConnection = nullptr;
	double closestDistance;
	for (UActorComponent* component : components)
	{
		UFGFactoryConnectionComponent* connection = Cast<UFGFactoryConnectionComponent>(component);

		if (!IsValid(connection))
		{
			continue;
		}

		// Transmitters can't connect to extractor connections that are already connected.
		if (connection->IsConnected())
		{
			continue;
		}

		// Fluid to fluid, conveyor to conveyor
		if (connection->GetConnector() != cachedTransmitterConnection->GetConnector())
		{
			continue;
		}
		
		if (closestConnection == nullptr)
		{
			closestConnection = connection;
			closestDistance = FVector::Distance(hitResult.ImpactPoint, connection->GetConnectorLocation());
		}
		else
		{
			double newDistance = FVector::Distance(hitResult.ImpactPoint, connection->GetConnectorLocation());
			if (newDistance < closestDistance)
			{
				closestDistance = newDistance;
				closestConnection = connection;
			}
		}
	}

	if (!IsValid(closestConnection))
	{
		return false;
	}

	cachedExtractorConnection = closestConnection;
	SnapToConnection(cachedExtractorConnection);

	return true;
}

void ATransmitterHologram::SnapToConnection(const UFGFactoryConnectionComponent* connection)
{
	// Finds the oposite rotation of the target by using the opposite forward vector.
	// FRotator targetRotation = (-connection->GetComponentRotation().Quaternion().GetForwardVector()).Rotation();
	//UE_LOG(LogTemp, Warning, TEXT("Connection position: %s\nOffset: %s"), *connection->GetComponentLocation().ToString(), *(GetActorLocation() - cachedTransmitterConnection->GetComponentLocation()).ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Snap position: %s"), *cachedTransmitterConnection->GetComponentLocation().ToString());
	FVector location = GetActorLocation() - cachedTransmitterConnection->GetComponentLocation() + connection->GetComponentLocation();
	FQuat rotation = connection->GetComponentRotation().Quaternion() * cachedTransmitterConnection->GetRelativeRotation().Quaternion().Inverse();
	SetActorLocationAndRotation(location, rotation);

	//parent.transform.rotation = target.transform.rotation * Quaternion.Inverse(child.transform.localRotation);
	//parent.transform.position = parent.transform.position - child.transform.position + target.transform.position;
}

void ATransmitterHologram::CheckValidPlacement()
{
	if (!IsValid(cachedExtractorConnection))
	{
		AddConstructDisqualifier(UMustSnapToMinerDisqualifier::StaticClass());
	}

	Super::CheckValidPlacement();
}

bool ATransmitterHologram::ShouldActorBeConsideredForGuidelines(AActor* actor) const
{
	return false;
}

TOptional<TSubclassOf<UFGRecipe>> ATransmitterHologram::ProcessHologramOverride(const FHitResult& hitResult) const
{
	//Called every frame. Check if the hitResult still points to the same actor and short-circuit.
	UE_LOG(LogRawResourceTeleporter, Display, TEXT("ProcessHologramOverride called"));

	ABuild_TransmitterBase* buildable = GetDefaultBuildable<ABuild_TransmitterBase>();

	//Make sure nothing weird happened.
	if (!buildable)
		return{};

	UHologramOverrides* hologramOverrides = buildable->GetHologramOverride();
	
	// If the player is looking at something, pass control to the HologramOverride data asset assigned to the buildable
	// and if it has one of the tags associated with a Transmitter attachment, return the override.
	if (AActor* hitActor = hitResult.GetActor())
	{
		TOptional<TSubclassOf<UFGRecipe>> result = hologramOverrides->GetHologramOverride(this, hitActor->GetClass());
		if (result.IsSet())
		{			
			return result.GetValue();
			//transmitterType = ETransmitterType::Attachment;
		}
	}

	// If either the player isn't looking at something, or it doesn't have a tag, then override with a standalone transmitter.
	return hologramOverrides->GetSolidStandaloneTransmitter();
}

// If this isn't overriden, then the base class will hide the hologram whenever pointed at a machine (which this only snaps to extractors, so this is a problem).
bool ATransmitterHologram::IsValidHitResult(const FHitResult& hitResult) const
{
	return true;
}
