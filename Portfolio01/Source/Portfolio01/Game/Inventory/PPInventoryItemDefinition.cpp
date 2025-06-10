// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPInventoryItemDefinition.h"

#include "UObject/Class.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPInventoryItemDefinition)

//////////////////////////////////////////////////////////////////////
// UPPInventoryItemDefinition

UPPInventoryItemDefinition::UPPInventoryItemDefinition(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

const UPPInventoryItemFragment* UPPInventoryItemDefinition::FindFragmentByClass(TSubclassOf<UPPInventoryItemFragment> FragmentClass) const
{
	if (FragmentClass != nullptr)
	{
		for (UPPInventoryItemFragment* Fragment : Fragments)
		{
			if (Fragment && Fragment->IsA(FragmentClass))
			{
				return Fragment;
			}
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////
// UPPInventoryItemDefinition

const UPPInventoryItemFragment* UPPInventoryFunctionLibrary::FindItemDefinitionFragment(TSubclassOf<UPPInventoryItemDefinition> ItemDef, TSubclassOf<UPPInventoryItemFragment> FragmentClass)
{
	if ((ItemDef != nullptr) && (FragmentClass != nullptr))
	{
		return GetDefault<UPPInventoryItemDefinition>(ItemDef)->FindFragmentByClass(FragmentClass);
	}
	return nullptr;
}

