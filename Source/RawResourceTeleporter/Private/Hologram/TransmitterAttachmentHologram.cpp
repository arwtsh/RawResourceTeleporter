#include "Hologram/TransmitterAttachmentHologram.h"
#include "Hologram/HologramOverrides.h"
#include "Hologram/TransmitterBuildModes.h"
#include "RawResourceTeleporter.h" //For logging


#include "FGFactoryConnectionComponent.h"
#include "MustSnapToMinerDisqualifier.h"


TOptional<TSubclassOf<UFGRecipe>> ATransmitterAttachmentHologram::GetHologramOverride(const UHologramOverrides* hologramOverrides, const AActor* hitActor) const
{
	TOptional<TSubclassOf<UFGRecipe>> result = Super::GetHologramOverride(hologramOverrides, hitActor);

	//The base class's GetHologramOverride only consideres new attachments.
	//To keep the same attachment, it will return the recipe the build gun is currently using
	//and the build gun won't change anything.
	//But if it returns nothing, then it shouldn't be using an attachment.
	//Since this is the attachment hologram, it needs to override to a standalone.
	if (!result.IsSet())
	{
		//Change which standalone gets overriden to based on the current build mode.
		if (IsCurrentBuildMode(USolidTransmitterBuildMode::StaticClass()))
		{
			result = hologramOverrides->GetSolidStandaloneTransmitter();
		}
		else if (IsCurrentBuildMode(UFluidTransmitterBuildMode::StaticClass()))
		{
			result = hologramOverrides->GetFluidStandaloneTransmitter();
		}
		else
		{
			UE_LOG(LogRawResourceTeleporter, Warning, TEXT("TransmitterAttachmentHologram tried to override to a standalone transmitter, but it found an unknown build mode! This hologram should only have Solid or Fluid build modes."));
		}
	}

	return result;
}

void ATransmitterAttachmentHologram::BeginPlay()
{
	Super::BeginPlay();

	// The hologram should only have 1 input connection. It will crash if there are none.
	cachedTransmitterConnection = GetCachedFactoryConnectionComponents()[0];

	// Can be placed hovering over a surface as long as it's attached to the extractor.
	SetNeedsValidFloor(false);
}

bool ATransmitterAttachmentHologram::TrySnapToActor(const FHitResult& hitResult)
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

void ATransmitterAttachmentHologram::SnapToConnection(const UFGFactoryConnectionComponent* connection)
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

void ATransmitterAttachmentHologram::CheckValidPlacement()
{
	if (!IsValid(cachedExtractorConnection))
	{
		AddConstructDisqualifier(UMustSnapToMinerDisqualifier::StaticClass());
	}

	Super::CheckValidPlacement();
}

bool ATransmitterAttachmentHologram::ShouldActorBeConsideredForGuidelines(AActor* actor) const
{
	return false;
}


