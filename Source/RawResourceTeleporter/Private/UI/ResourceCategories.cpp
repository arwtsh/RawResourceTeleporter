// 

#include "UI/ResourceCategories.h"

#include "FGBlueprintFunctionLibrary.h"
#include "RawResourceTeleporter.h"

TArray<FCategoryData> UResourceCategories::GetAllCategories() const
{
	return Categories;
}

void UResourceCategories::ParseResourceCategoriesFiltered(UObject* WorldContextObject, const FParseResourceCategoriesCallback& Callback, const TSet<TSoftClassPtr<UFGItemDescriptor>>& AllowList)
{
	TSet<TSoftClassPtr<UFGItemDescriptor>> parsedResources;
	for (const auto& [CategoryName, Resources] : Categories)
	{
		if (Resources.Num() <= 0)
		{
			UE_LOG(LogRawResourceTeleporter, Warning, TEXT("Category %s doesn't have any associated Resources!."), *CategoryName.ToString())
		}
		
		TArray<TSubclassOf<UFGItemDescriptor>> categoryResources;
		for (int i = 0; i < Resources.Num(); i++)
		{
			const TSoftClassPtr<UFGItemDescriptor>& Resource = Resources[i];
			
			if (parsedResources.Contains(Resource))
			{
				UE_LOG(LogRawResourceTeleporter, Error, TEXT("Resource %s was parsed multiple times! It might appear in multiple different categories."), *Resource.GetAssetName())
				continue;
			}

			if (!AllowList.Contains(Resource))
			{
				//This is an expected time to ignore resources. For example, Recievers only accept whitelisted resources.
				continue;
			}

			//Make sure the asset actually exists, SoftClassPointer are just strings to asset locations.
			//If the asset is deleted or inputted incorrectly in a ContentLib CDO json, then it will be invalid.
			TSubclassOf<UFGItemDescriptor> loadedResource = Resource.LoadSynchronous();
			if (!IsValid(loadedResource))
			{
				UE_LOG(LogRawResourceTeleporter, Error, TEXT("The resource class at index %d in category %s was invalid."), i, *CategoryName.ToString())
				continue;
			}
			
			categoryResources.Add(loadedResource);
			parsedResources.Add(Resource); //Make sure this resource doesn't get added again.
		}

		//This only happens when the AllowList filters all the items in this category.
		//(unless the only items were duplicates from an earlier category, but this would be logged already)
		if (categoryResources.Num() <= 0)
		{
			continue;
		}
		
		Callback.ExecuteIfBound(CategoryName, categoryResources);
	}

	//If there's any items not tracked by this data asset, add them to an Uncategorized category.
	TArray<TSubclassOf<UFGItemDescriptor>> allDescriptors;
	UFGBlueprintFunctionLibrary::GetAllDescriptorsSorted(WorldContextObject, allDescriptors);
	TArray<TSubclassOf<UFGItemDescriptor>> extraResources;
	for (const TSubclassOf<UFGItemDescriptor>& descriptor : allDescriptors)
	{
		TSoftClassPtr<UFGItemDescriptor> resource = TSoftClassPtr<UFGItemDescriptor>(descriptor);
		if (!AllowList.Contains(resource) || parsedResources.Contains(resource) || Ignore.Contains(resource))
		{
			continue;
		}

		extraResources.Add(descriptor);
	}
	if (extraResources.Num() > 0)
	{
		Callback.ExecuteIfBound(FText::FromString(TEXT("Uncategorized")), extraResources);
	}
}

void UResourceCategories::ParseResourceCategories(UObject* WorldContextObject, const FParseResourceCategoriesCallback& Callback)
{
	TSet<TSoftClassPtr<UFGItemDescriptor>> parsedResources;
	for (const auto& [CategoryName, Resources] : Categories)
	{
		if (Resources.Num() <= 0)
		{
			UE_LOG(LogRawResourceTeleporter, Warning, TEXT("Category %s doesn't have any associated Resources!."), *CategoryName.ToString())
		}
		
		TArray<TSubclassOf<UFGItemDescriptor>> categoryResources;
		for (int i = 0; i < Resources.Num(); i++)
		{
			const TSoftClassPtr<UFGItemDescriptor>& Resource = Resources[i];
			
			if (parsedResources.Contains(Resource))
			{
				UE_LOG(LogRawResourceTeleporter, Error, TEXT("Resource %s was parsed multiple times! It might appear in multiple different categories."), *Resource.GetAssetName())
				continue;
			}

			//Make sure the asset actually exists, SoftClassPointer are just strings to asset locations.
			//If the asset is deleted or inputted incorrectly in a ContentLib CDO json, then it will be invalid.
			TSubclassOf<UFGItemDescriptor> loadedResource = Resource.LoadSynchronous();
			if (!IsValid(loadedResource))
			{
				UE_LOG(LogRawResourceTeleporter, Error, TEXT("The resource class at index %d in category %s was invalid."), i, *CategoryName.ToString())
				continue;
			}
			
			categoryResources.Add(loadedResource);
			parsedResources.Add(Resource); //Make sure this resource doesn't get added again.
		}

		//This only happens when the AllowList filters all the items in this category.
		//(unless the only items were duplicates from an earlier category, but this would be logged already)
		if (categoryResources.Num() <= 0)
		{
			continue;
		}

		Callback.ExecuteIfBound(CategoryName, categoryResources);
	}

	//If there's any items not tracked by this data asset, add them to an Uncategorized category.
	TArray<TSubclassOf<UFGItemDescriptor>> allDescriptors;
	UFGBlueprintFunctionLibrary::GetAllDescriptorsSorted(WorldContextObject, allDescriptors);
	TArray<TSubclassOf<UFGItemDescriptor>> extraResources;
	for (const TSubclassOf<UFGItemDescriptor>& descriptor : allDescriptors)
	{
		TSoftClassPtr<UFGItemDescriptor> resource = TSoftClassPtr<UFGItemDescriptor>(descriptor);
		if (parsedResources.Contains(resource) || Ignore.Contains(resource))
		{
			continue;
		}

		extraResources.Add(descriptor);
	}
	if (extraResources.Num() > 0)
	{
		Callback.ExecuteIfBound(FText::FromString(TEXT("Uncategorized")), extraResources);
	}
}
