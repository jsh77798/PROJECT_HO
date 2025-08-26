// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbilitySpec.h"
#include "PPAbilityCost.h"
#include "ScalableFloat.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

#include "PPAbilityCost_InventoryItem.generated.h"

class UPPGameplayAbility;
class UPPInventoryItemDefinition;
class UObject;
struct FGameplayAbilityActorInfo;
struct FGameplayTagContainer;

/**
 * Represents a cost that requires expending a quantity of an inventory item
 */
UCLASS(meta=(DisplayName="Inventory Item"))
class UPPAbilityCost_InventoryItem : public UPPAbilityCost
{
	GENERATED_BODY()

public:
	UPPAbilityCost_InventoryItem();

	//~UPPAbilityCost interface
	virtual bool CheckCost(const UPPGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* OptionalRelevantTags) const override;
	virtual void ApplyCost(const UPPGameplayAbility* Ability, const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) override;
	//~End of UPPAbilityCost interface

protected:
	/** How much of the item to spend (keyed on ability level) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	FScalableFloat Quantity;

	/** Which item to consume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=AbilityCost)
	TSubclassOf<UPPInventoryItemDefinition> ItemDefinition;
};
