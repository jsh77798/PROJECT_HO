// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPAbilityCost_InventoryItem.h"

#include "PPGameplayAbility.h"
#include "Inventory/PPInventoryManagerComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPAbilityCost_InventoryItem)

UPPAbilityCost_InventoryItem::UPPAbilityCost_InventoryItem()
{
	Quantity.SetValue(1.0f);
}

bool UPPAbilityCost_InventoryItem::CheckCost(const UPPGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const
{
#if 0
	if (AController* PC = Ability->GetControllerFromActorInfo())
	{
		if (UPPInventoryManagerComponent* InventoryComponent = PC->GetComponentByClass<UPPInventoryManagerComponent>())
		{
			const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

			const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
			const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

			return InventoryComponent->GetTotalItemCountByDefinition(ItemDefinition) >= NumItemsToConsume;
		}
	}
#endif
	return false;
}

void UPPAbilityCost_InventoryItem::ApplyCost(const UPPGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo)
{
#if 0
	if (ActorInfo->IsNetAuthority())
	{
		if (AController* PC = Ability->GetControllerFromActorInfo())
		{
			if (UPPInventoryManagerComponent* InventoryComponent = PC->GetComponentByClass<UPPInventoryManagerComponent>())
			{
				const int32 AbilityLevel = Ability->GetAbilityLevel(Handle, ActorInfo);

				const float NumItemsToConsumeReal = Quantity.GetValueAtLevel(AbilityLevel);
				const int32 NumItemsToConsume = FMath::TruncToInt(NumItemsToConsumeReal);

				InventoryComponent->ConsumeItemsByDefinition(ItemDefinition, NumItemsToConsume);
			}
		}
	}
#endif
}

