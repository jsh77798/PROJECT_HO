// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPGameplayAbility_FromEquipment.h"
#include "PPEquipmentInstance.h"
#include "Game/Inventory/PPInventoryItemInstance.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPGameplayAbility_FromEquipment)

UPPGameplayAbility_FromEquipment::UPPGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

UPPEquipmentInstance* UPPGameplayAbility_FromEquipment::GetAssociatedEquipment() const
{
	if (FGameplayAbilitySpec* Spec = UGameplayAbility::GetCurrentAbilitySpec())
	{
		return Cast<UPPEquipmentInstance>(Spec->SourceObject.Get());
	}

	return nullptr;
}

UPPInventoryItemInstance* UPPGameplayAbility_FromEquipment::GetAssociatedItem() const
{
	if (UPPEquipmentInstance* Equipment = GetAssociatedEquipment())
	{
		return Cast<UPPInventoryItemInstance>(Equipment->GetInstigator());
	}
	return nullptr;
}


#if WITH_EDITOR
EDataValidationResult UPPGameplayAbility_FromEquipment::IsDataValid(TArray<FText>& ValidationErrors)
{
	EDataValidationResult Result = Super::IsDataValid(ValidationErrors);

	if (InstancingPolicy == EGameplayAbilityInstancingPolicy::NonInstanced)
	{
		ValidationErrors.Add(NSLOCTEXT("PP", "EquipmentAbilityMustBeInstanced", "Equipment ability must be instanced"));
		Result = EDataValidationResult::Invalid;
	}

	return Result;
}

#endif
