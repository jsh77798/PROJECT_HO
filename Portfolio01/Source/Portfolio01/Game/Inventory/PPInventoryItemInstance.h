// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "HAL/Platform.h"
//#include "System/GameplayTagStack.h"
#include "Templates/SubclassOf.h"
#include "UObject/Object.h"
#include "UObject/UObjectGlobals.h"

#include "PPInventoryItemInstance.generated.h"

class UPPInventoryItemDefinition;
class UPPInventoryItemFragment;
struct FFrame;
struct FGameplayTag;

/**
 * UPPInventoryItemInstance
 */
UCLASS(BlueprintType)
class UPPInventoryItemInstance : public UObject
{
	GENERATED_BODY()
public:
	UPPInventoryItemInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//TSubclassOf<UPPInventoryItemDefinition> GetItemDef() const
	//{
	//	return ItemDef;
	//}

	UFUNCTION(BlueprintCallable, BlueprintPure=false, meta=(DeterminesOutputType=FragmentClass))
	const UPPInventoryItemFragment* FindFragmentByClass(TSubclassOf<UPPInventoryItemFragment> FragmentClass) const;

	template <typename ResultClass>
	const ResultClass* FindFragmentByClass() const
	{
		return (ResultClass*)FindFragmentByClass(ResultClass::StaticClass());
	}

private:
	void SetItemDef(TSubclassOf<UPPInventoryItemDefinition> InDef);

	//friend struct FPPInventoryList;

	/*
private:
	// The item definition
	UPROPERTY(Replicated)
	TSubclassOf<UPPInventoryItemDefinition> ItemDef;
	*/
};
