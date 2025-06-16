// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPInventoryManagerComponent.h"

#include "Engine/ActorChannel.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "GameFramework/GameplayMessageSubsystem.h"
#include "PPInventoryItemDefinition.h"
#include "PPInventoryItemInstance.h"
#include "Misc/AssertionMacros.h"
#include "NativeGameplayTags.h"
#include "UObject/NameTypes.h"
#include "UObject/Object.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectBaseUtility.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPInventoryManagerComponent)

UE_DEFINE_GAMEPLAY_TAG_STATIC(TAG_PP_Inventory_Message_StackChanged, "PP.Inventory.Message.StackChanged");

//////////////////////////////////////////////////////////////////////
// FPPInventoryEntry

FString FPPInventoryEntry::GetDebugString() const
{
	TSubclassOf<UPPInventoryItemDefinition> ItemDef;
	if (Instance != nullptr)
	{
		ItemDef = Instance->GetItemDef();
	}

	return FString::Printf(TEXT("%s (%d x %s)"), *GetNameSafe(Instance), StackCount, *GetNameSafe(ItemDef));
}

//////////////////////////////////////////////////////////////////////
// FPPInventoryList

void FPPInventoryList::BroadcastChangeMessage(FPPInventoryEntry& Entry, int32 OldCount, int32 NewCount)
{
	if (!OwnerComponent || !Entry.Instance) { return; }

	FPPInventoryChangeMessage Message;
	Message.InventoryOwner = OwnerComponent;
	Message.Instance = Entry.Instance;
	Message.NewCount = NewCount;
	Message.Delta = NewCount - OldCount;

	UGameplayMessageSubsystem& MessageSystem = UGameplayMessageSubsystem::Get(OwnerComponent->GetWorld());
	MessageSystem.BroadcastMessage(TAG_PP_Inventory_Message_StackChanged, Message);
}

UPPInventoryItemInstance* FPPInventoryList::AddEntry(TSubclassOf<UPPInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UPPInventoryItemInstance* Result = nullptr;

	check(ItemDef != nullptr);
 	check(OwnerComponent);

	AActor* OwningActor = OwnerComponent->GetOwner();

	FPPInventoryEntry& NewEntry = Entries.AddDefaulted_GetRef();
	NewEntry.Instance = NewObject<UPPInventoryItemInstance>(OwningActor);  //@TODO: Using the actor instead of component as the outer due to UE-127172
	NewEntry.Instance->SetItemDef(ItemDef);
	for (UPPInventoryItemFragment* Fragment : GetDefault<UPPInventoryItemDefinition>(ItemDef)->Fragments)
	{
		if (Fragment != nullptr)
		{
			Fragment->OnInstanceCreated(NewEntry.Instance);
		}
	}
	NewEntry.StackCount = StackCount;
	Result = NewEntry.Instance;

	return Result;
}

void FPPInventoryList::AddEntry(UPPInventoryItemInstance* Instance)
{
	unimplemented();
}

void FPPInventoryList::RemoveEntry(UPPInventoryItemInstance* Instance)
{
	for (auto EntryIt = Entries.CreateIterator(); EntryIt; ++EntryIt)
	{
		FPPInventoryEntry& Entry = *EntryIt;
		if (Entry.Instance == Instance)
		{
			EntryIt.RemoveCurrent();
		}
	}
}

TArray<UPPInventoryItemInstance*> FPPInventoryList::GetAllItems() const
{
	TArray<UPPInventoryItemInstance*> Results;
	Results.Reserve(Entries.Num());
	for (const FPPInventoryEntry& Entry : Entries)
	{
		if (Entry.Instance != nullptr) //@TODO: Would prefer to not deal with this here and hide it further?
		{
			Results.Add(Entry.Instance);
		}
	}
	return Results;
}

//////////////////////////////////////////////////////////////////////
// UPPInventoryManagerComponent

UPPInventoryManagerComponent::UPPInventoryManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
	, InventoryList(this)
{
}

bool UPPInventoryManagerComponent::CanAddItemDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef, int32 StackCount)
{
	//@TODO: Add support for stack limit / uniqueness checks / etc...
	return true;
}

UPPInventoryItemInstance* UPPInventoryManagerComponent::AddItemDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef, int32 StackCount)
{
	UPPInventoryItemInstance* Result = nullptr;
	if (ItemDef != nullptr)
	{
		Result = InventoryList.AddEntry(ItemDef, StackCount);
	}
	return Result;
}

void UPPInventoryManagerComponent::AddItemInstance(UPPInventoryItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		InventoryList.AddEntry(ItemInstance);
	}
}

void UPPInventoryManagerComponent::RemoveItemInstance(UPPInventoryItemInstance* ItemInstance)
{
	if (ItemInstance)
	{
		InventoryList.RemoveEntry(ItemInstance);
	}
}

TArray<UPPInventoryItemInstance*> UPPInventoryManagerComponent::GetAllItems() const
{
	return InventoryList.GetAllItems();
}

UPPInventoryItemInstance* UPPInventoryManagerComponent::FindFirstItemStackByDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef) const
{
	for (const FPPInventoryEntry& Entry : InventoryList.Entries)
	{
		UPPInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				return Instance;
			}
		}
	}

	return nullptr;
}

int32 UPPInventoryManagerComponent::GetTotalItemCountByDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef) const
{
	int32 TotalCount = 0;
	for (const FPPInventoryEntry& Entry : InventoryList.Entries)
	{
		UPPInventoryItemInstance* Instance = Entry.Instance;

		if (IsValid(Instance))
		{
			if (Instance->GetItemDef() == ItemDef)
			{
				++TotalCount;
			}
		}
	}

	return TotalCount;
}

bool UPPInventoryManagerComponent::ConsumeItemsByDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef, int32 NumToConsume)
{
	if (ItemDef == nullptr || NumToConsume <= 0)
	{
		return false;
	}

	//@TODO: N squared right now as there's no acceleration structure
	int32 TotalConsumed = 0;
	while (TotalConsumed < NumToConsume)
	{
		if (UPPInventoryItemInstance* Instance = UPPInventoryManagerComponent::FindFirstItemStackByDefinition(ItemDef))
		{
			InventoryList.RemoveEntry(Instance);
			++TotalConsumed;
		}
		else
		{
			// 더 이상 해당 아이템이 없어서 중단
			return false;
		}
	}

	return TotalConsumed == NumToConsume;
}

//////////////////////////////////////////////////////////////////////
//

// UCLASS(Abstract)
// class ULyraInventoryFilter : public UObject
// {
// public:
// 	virtual bool PassesFilter(ULyraInventoryItemInstance* Instance) const { return true; }
// };

// UCLASS()
// class ULyraInventoryFilter_HasTag : public ULyraInventoryFilter
// {
// public:
// 	virtual bool PassesFilter(ULyraInventoryItemInstance* Instance) const { return true; }
// };


