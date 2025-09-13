//

#include "RawResourceTeleporterRCO.h"
#include "Build_ReceiverBase.h"
#include "FGPlayerController.h"
#include "UnrealNetwork.h"
#include "RawResourceTeleporter.h"


void URawResourceTeleporterRCO::Server_SetReceiverResourceToTeleport_Implementation(ABuild_ReceiverBase* Receiver, TSubclassOf<UFGItemDescriptor> Resource)
{
	Receiver->EjectInventory(GetOwnerPlayerCharacter());

	Receiver->SetResourceToTeleport(Resource);
	Receiver->ForceNetUpdate();
}

URawResourceTeleporterRCO* URawResourceTeleporterRCO::Get(const UObject* WorldContext)
{
	if (WorldContext)
	{
		if (AFGPlayerController* Controller = Cast<AFGPlayerController>(WorldContext->GetWorld()->GetFirstPlayerController()))
		{
			return Controller->GetRemoteCallObjectOfClass<URawResourceTeleporterRCO>();
		}
	}

	return nullptr;
}

void URawResourceTeleporterRCO::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URawResourceTeleporterRCO, bDummy);
}
