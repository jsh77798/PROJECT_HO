// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPGameplayEffectContext.h"

#include "Game/AbilitySystem/PPAbilitySourceInterface.h"
#include "Engine/HitResult.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Templates/Casts.h"
#include "UObject/Object.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(PPGameplayEffectContext)

class FArchive;

FPPGameplayEffectContext* FPPGameplayEffectContext::ExtractEffectContext(struct FGameplayEffectContextHandle Handle)
{
	FGameplayEffectContext* BaseEffectContext = Handle.Get();
	if ((BaseEffectContext != nullptr) && BaseEffectContext->GetScriptStruct()->IsChildOf(FPPGameplayEffectContext::StaticStruct()))
	{
		return (FPPGameplayEffectContext*)BaseEffectContext;
	}

	return nullptr;
}

void FPPGameplayEffectContext::SetAbilitySource(const IPPAbilitySourceInterface* InObject, float InSourceLevel)
{
	AbilitySourceObject = MakeWeakObjectPtr(Cast<const UObject>(InObject));
	//SourceLevel = InSourceLevel;
}

const IPPAbilitySourceInterface* FPPGameplayEffectContext::GetAbilitySource() const
{
	return Cast<IPPAbilitySourceInterface>(AbilitySourceObject.Get());
}

const UPhysicalMaterial* FPPGameplayEffectContext::GetPhysicalMaterial() const
{
	if (const FHitResult* HitResultPtr = GetHitResult())
	{
		return HitResultPtr->PhysMaterial.Get();
	}
	return nullptr;
}

