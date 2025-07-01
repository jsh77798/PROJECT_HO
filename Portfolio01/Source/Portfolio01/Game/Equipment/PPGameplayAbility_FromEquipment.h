// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Game/AbilitySystem/Abilities/PPGameplayAbility.h"

#include "PPGameplayAbility_FromEquipment.generated.h"

class UPPEquipmentInstance;
class UPPInventoryItemInstance;

/**
 * UPPGameplayAbility_FromEquipment
 *
 * An ability granted by and associated with an equipment instance
 */
UCLASS()
class UPPGameplayAbility_FromEquipment : public UPPGameplayAbility
{
	GENERATED_BODY()

public:

	UPPGameplayAbility_FromEquipment(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category="Ability")
	UPPEquipmentInstance* GetAssociatedEquipment() const;

	UFUNCTION(BlueprintCallable, Category = "Ability")
	UPPInventoryItemInstance* GetAssociatedItem() const;

#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif

};
