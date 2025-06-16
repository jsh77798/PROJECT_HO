// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/ActorComponent.h"
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "Containers/Map.h"
#include "Containers/Set.h"
#include "Containers/SparseArray.h"
#include "Containers/UnrealString.h"
#include "CoreTypes.h"
#include "Templates/SubclassOf.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/Class.h"
#include "UObject/UObjectGlobals.h"

#include "PPInventoryManagerComponent.generated.h"

class UPPInventoryItemDefinition;
class UPPInventoryItemInstance;
class UPPInventoryManagerComponent;
class UObject;
struct FFrame;
struct FPPInventoryList;

/** A message when an item is added to the inventory */
USTRUCT(BlueprintType)
struct FPPInventoryChangeMessage
{
	GENERATED_BODY()

	//@TODO: Tag based names+owning actors for inventories instead of directly exposing the component?
	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	TObjectPtr<UActorComponent> InventoryOwner = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = Inventory)
	TObjectPtr<UPPInventoryItemInstance> Instance = nullptr;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 NewCount = 0;

	UPROPERTY(BlueprintReadOnly, Category=Inventory)
	int32 Delta = 0;
};

/** A single entry in an inventory */
USTRUCT(BlueprintType)
struct FPPInventoryEntry 
{
	GENERATED_BODY()

	FPPInventoryEntry()
	{}

	FString GetDebugString() const;

private:
	friend FPPInventoryList;
	friend UPPInventoryManagerComponent;

	UPROPERTY()
	TObjectPtr<UPPInventoryItemInstance> Instance = nullptr;

	UPROPERTY()
	int32 StackCount = 0;

	//UPROPERTY(NotReplicated)
	//int32 LastObservedCount = INDEX_NONE;
};

/** List of inventory items */
USTRUCT(BlueprintType)
struct FPPInventoryList 
{
	GENERATED_BODY()

	FPPInventoryList()
		: OwnerComponent(nullptr)
	{
	}

	FPPInventoryList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	TArray<UPPInventoryItemInstance*> GetAllItems() const;

public:
	
	UPPInventoryItemInstance* AddEntry(TSubclassOf<UPPInventoryItemDefinition> ItemClass, int32 StackCount);
	void AddEntry(UPPInventoryItemInstance* Instance);

	void RemoveEntry(UPPInventoryItemInstance* Instance);

private:
	void BroadcastChangeMessage(FPPInventoryEntry& Entry, int32 OldCount, int32 NewCount);

private:
	friend UPPInventoryManagerComponent;

private:
	// list of items
	UPROPERTY()
	TArray<FPPInventoryEntry> Entries;

	UPROPERTY()
	TObjectPtr<UActorComponent> OwnerComponent;
};

//template<>
//struct TStructOpsTypeTraits<FPPInventoryList> : public TStructOpsTypeTraitsBase2<FPPInventoryList>
//{
	//enum { WithNetDeltaSerializer = true };
//};










/**
 * Manages an inventory
 */
UCLASS(BlueprintType)
class PORTFOLIO01_API UPPInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPPInventoryManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable, Category = Inventory)
	bool CanAddItemDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	UPPInventoryItemInstance* AddItemDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef, int32 StackCount = 1);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void AddItemInstance(UPPInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	void RemoveItemInstance(UPPInventoryItemInstance* ItemInstance);

	UFUNCTION(BlueprintCallable, Category = Inventory)
	TArray<UPPInventoryItemInstance*> GetAllItems() const;

	UFUNCTION(BlueprintCallable, Category = Inventory)
	UPPInventoryItemInstance* FindFirstItemStackByDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef) const;

	int32 GetTotalItemCountByDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef) const;
	bool ConsumeItemsByDefinition(TSubclassOf<UPPInventoryItemDefinition> ItemDef, int32 NumToConsume);

	//~UObject interface
	//virtual bool ReplicateSubobjects(class UActorChannel* Channel, class FOutBunch* Bunch, FReplicationFlags* RepFlags) override;
	//virtual void ReadyForReplication() override;
	//~End of UObject interface

private:
	UPROPERTY()
	FPPInventoryList InventoryList;
};
