// 


#include "Build_RecieverBase.h"


// Sets default values
ABuild_RecieverBase::ABuild_RecieverBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SaveConfig();
}

TSet<TSoftClassPtr<UFGItemDescriptor>> ABuild_RecieverBase::GetAllowedTeleportableItems_Implementation() const
{
	UE_LOG(LogTemp, Warning, TEXT("GetAllowedTeleportableItems's default implementation was called."));
	return TSet<TSoftClassPtr<UFGItemDescriptor>>();
}
