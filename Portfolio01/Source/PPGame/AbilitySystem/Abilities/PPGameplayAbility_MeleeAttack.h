// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "GameplayAbilitySpec.h"
#include "PPGameplayAbility.h"
#include "UObject/UObjectGlobals.h"

#include "PPGameplayAbility_MeleeAttack.generated.h"

class UObject;
struct FFrame;
struct FGameplayAbilityActorInfo;
struct FGameplayEventData;


/**
 * UPPGameplayAbility_MeleeAttack
 *
 */
UCLASS(Abstract)
class UPPGameplayAbility_MeleeAttack : public UPPGameplayAbility
{
	GENERATED_BODY()

public:

	UPPGameplayAbility_MeleeAttack(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

};
