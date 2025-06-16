// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPInventoryItemInstance.h"

#include "Containers/Array.h"
#include "GameplayTagContainer.h"
#include "PPInventoryItemDefinition.h"
#include "Misc/AssertionMacros.h"
#include "UObject/Class.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPInventoryItemInstance)

UPPInventoryItemInstance::UPPInventoryItemInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPPInventoryItemInstance::SetItemDef(TSubclassOf<UPPInventoryItemDefinition> InDef)
{
	ItemDef = InDef;
}

const UPPInventoryItemFragment* UPPInventoryItemInstance::FindFragmentByClass(TSubclassOf<UPPInventoryItemFragment> FragmentClass) const
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UPPInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

