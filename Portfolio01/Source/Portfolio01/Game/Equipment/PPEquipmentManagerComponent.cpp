// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPEquipmentManagerComponent.h"

#include "Game/AbilitySystem/PPAbilitySet.h"
#include "Game/AbilitySystem/PPAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Components/ActorComponent.h"
#include "Engine/ActorChannel.h"
#include "GameFramework/Actor.h"
#include "PPEquipmentDefinition.h"
#include "PPEquipmentInstance.h"
#include "Misc/AssertionMacros.h"
//#include "Net/UnrealNetwork.h"
#include "Templates/Casts.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectBaseUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPEquipmentManagerComponent)

//////////////////////////////////////////////////////////////////////
// FPPAppliedEquipmentEntry

FString FPPAppliedEquipmentEntry::GetDebugString() const
{
	return FString::Printf(TEXT("%s of %s"), *GetNameSafe(Instance), *GetNameSafe(EquipmentDefinition.Get()));
}

//////////////////////////////////////////////////////////////////////
// FPPEquipmentList

UPPAbilitySystemComponent* FPPEquipmentList::GetAbilitySystemComponent() const
{
	check(OwnerComponent);
	AActor* OwningActor = OwnerComponent->GetOwner();
	return Cast<UPPAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor));
}

UPPEquipmentInstance* FPPEquipmentList::AddEntry(TSubclassOf<UPPEquipmentDefinition> EquipmentDefinition)
{
	UPPEquipmentInstance* Result = nullptr;

	check(EquipmentDefinition != nullptr);
 	check(OwnerComponent);
	
	const UPPEquipmentDefinition* EquipmentCDO = GetDefault<UPPEquipmentDefinition>(EquipmentDefinition);

	TSubclassOf<UPPEquipmentInstance> InstanceType = EquipmentCDO->InstanceType;
	if (InstanceType == nullptr)
	{
		InstanceType = UPPEquipmentInstance::StaticClass();
	}
	
	FPPAppliedEquipmentEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.EquipmentDefinition = EquipmentDefinition;
	NewEntry.Instance = NewObject<UPPEquipmentInstance>(OwnerComponent->GetOwner(), InstanceType);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	Result = NewEntry.Instance;

	if (UPPAbilitySystemComponent* ASC = GetAbilitySystemComponent())
	{
		for (TObjectPtr<const UPPAbilitySet> AbilitySet : EquipmentCDO->AbilitySetsToGrant)
		{
			AbilitySet->GiveToAbilitySystem(ASC, /*inout*/ &NewEntry.GrantedHandles, Result);
		}
	}
	else
	{
		//@TODO: Warning logging?
	}

	Result->SpawnEquipmentActors(EquipmentCDO->ActorsToSpawn);

	return Result;
}

void FPPEquipmentList::RemoveEntry(UPPEquipmentInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPPAppliedEquipmentEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			if (UPPAbilitySystemComponent* ASC = GetAbilitySystemComponent())
			{
				Entry.GrantedHandles.TakeFromAbilitySystem(ASC);
			}

			Instance->DestroyEquipmentActors();

			EntryIt.RemoveCurrent();
		}
	}
}

//////////////////////////////////////////////////////////////////////
// UPPEquipmentManagerComponent

UPPEquipmentManagerComponent::UPPEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, EquipmentList(this)
{
	bWantsInitializeComponent = true;
}

UPPEquipmentInstance* UPPEquipmentManagerComponent::EquipItem(TSubclassOf<UPPEquipmentDefinition> EquipmentClass)
{
	UPPEquipmentInstance* Result = nullptr;
	if (EquipmentClass != nullptr)
	{
		Result = EquipmentList.AddEntry(EquipmentClass);
		if (Result != nullptr)
		{
			Result->OnEquipped();
		}
	}
	return Result;
}

void UPPEquipmentManagerComponent::UnequipItem(UPPEquipmentInstance* ItemInstance)
{
	if (ItemInstance != nullptr)
	{
		ItemInstance->OnUnequipped();
		EquipmentList.RemoveEntry(ItemInstance);
	}
}

void UPPEquipmentManagerComponent::InitializeComponent()
{
	Super::InitializeComponent();
}

void UPPEquipmentManagerComponent::UninitializeComponent()
{
	TArray<UPPEquipmentInstance*> AllEquipmentInstances;

	// gathering all instances before removal to avoid side effects affecting the equipment list iterator	
	for (const FPPAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		AllEquipmentInstances.Add(Entry.Instance);
	}

	for (UPPEquipmentInstance* EquipInstance : AllEquipmentInstances)
	{
		UnequipItem(EquipInstance);
	}

	Super::UninitializeComponent();
}

UPPEquipmentInstance* UPPEquipmentManagerComponent::GetFirstInstanceOfType(TSubclassOf<UPPEquipmentInstance> InstanceType)
{
	for (FPPAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UPPEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

TArray<UPPEquipmentInstance*> UPPEquipmentManagerComponent::GetEquipmentInstancesOfType(TSubclassOf<UPPEquipmentInstance> InstanceType) const
{
	TArray<UPPEquipmentInstance*> Results;
	for (const FPPAppliedEquipmentEntry& Entry : EquipmentList.Entries)
	{
		if (UPPEquipmentInstance* Instance = Entry.Instance)
		{
			if (Instance->IsA(InstanceType))
			{
				Results.Add(Instance);
			}
		}
	}
	return Results;
}


