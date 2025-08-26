// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Cosmetics/PPCosmeticAnimationTypes.h"
#include "Equipment/PPEquipmentInstance.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

#include "PPWeaponInstance.generated.h"

class UAnimInstance;
class UObject;
struct FFrame;
struct FGameplayTagContainer;

/**
 * UPPWeaponInstance
 *
 * A piece of equipment representing a weapon spawned and applied to a pawn
 */
UCLASS()
class UPPWeaponInstance : public UPPEquipmentInstance
{
	GENERATED_BODY()

public:
	UPPWeaponInstance(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UPPEquipmentInstance interface
	virtual void OnEquipped();
	virtual void OnUnequipped();
	//~End of UPPEquipmentInstance interface

	UFUNCTION(BlueprintCallable)
	void UpdateFiringTime();

	// Returns how long it's been since the weapon was interacted with (fired or equipped)
	UFUNCTION(BlueprintPure)
	float GetTimeSinceLastInteractedWith() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FPPAnimLayerSelectionSet EquippedAnimSet;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Animation)
	FPPAnimLayerSelectionSet UneuippedAnimSet;

	// Choose the best layer from EquippedAnimSet or UneuippedAnimSet based on the specified gameplay tags
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Animation)
	TSubclassOf<UAnimInstance> PickBestAnimLayer(bool bEquipped, const FGameplayTagContainer& CosmeticTags) const;

private:
	double TimeLastEquipped = 0.0;
	double TimeLastFired = 0.0;
};
