// 

#pragma once

#include "CoreMinimal.h"
#include "FGItemDescriptor.h"
#include "Runtime/Engine/Classes/Engine/DataAsset.h"
#include "ResourceCategories.generated.h"

DECLARE_DYNAMIC_DELEGATE_TwoParams( FParseResourceCategoriesCallback, FText, CategoryName, const TArray<TSubclassOf<UFGItemDescriptor>>&, Resources );

USTRUCT(BlueprintType)
struct FCategoryData {
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText CategoryName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftClassPtr<UFGItemDescriptor>> Resources;
};

/**
 * 
 */
UCLASS(BlueprintType)
class RAWRESOURCETELEPORTER_API UResourceCategories : public UDataAsset
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly)
	TArray<FCategoryData> Categories;

	//The resources to ignore when adding untracked items from GetAllDescriptorsSorted.
	//This is helpful if there's an unobtainable item that gets auto-added.
	UPROPERTY(EditDefaultsOnly)
	TSet<TSoftClassPtr<UFGItemDescriptor>> Ignore;

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<FCategoryData> GetAllCategories() const;

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "Callback, ResourcesToIgnore"))
	void ParseResourceCategoriesFiltered(UObject* WorldContextObject, const FParseResourceCategoriesCallback& Callback, const TSet<TSoftClassPtr<UFGItemDescriptor>>& AllowList);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", AutoCreateRefTerm = "Callback, ResourcesToIgnore"))
	void ParseResourceCategories(UObject* WorldContextObject, const FParseResourceCategoriesCallback& Callback);
};
