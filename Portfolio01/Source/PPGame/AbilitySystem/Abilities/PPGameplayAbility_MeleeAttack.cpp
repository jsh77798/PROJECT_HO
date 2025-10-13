// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPGameplayAbility_MeleeAttack.h"

#include "Abilities/GameplayAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "AbilitySystem/Abilities/PPGameplayAbility.h"
#include "AbilitySystem/PPAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "Character/HealthComponent.h"
#include "Containers/Array.h"
#include "Containers/EnumAsByte.h"
#include "Containers/UnrealString.h"
#include "Delegates/Delegate.h"
#include "GameplayTagContainer.h"
#include "GameplayTagsManager.h"
#include "HAL/Platform.h"
#include "Logging/LogCategory.h"
#include "Logging/LogMacros.h"
#include "PPGameplayTags.h"
#include "LogChannels.h"
#include "Misc/AssertionMacros.h"
#include "Templates/Casts.h"
#include "Trace/Detail/Channel.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPGameplayAbility_MeleeAttack)

UPPGameplayAbility_MeleeAttack::UPPGameplayAbility_MeleeAttack(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}
