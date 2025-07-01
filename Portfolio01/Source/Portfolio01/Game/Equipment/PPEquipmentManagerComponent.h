// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Game/AbilitySystem/PPAbilitySet.h"
#include "Components/PawnComponent.h"
#include "Containers/Array.h"
//#include "Containers/ArrayView.h"
//#include "Containers/Map.h"
//#include "Containers/Set.h"
//#include "Containers/SparseArray.h"
//#include "Containers/UnrealString.h"
//#include "HAL/Platform.h"
//#include "Net/Serialization/FastArraySerializer.h"
#include "Templates/SubclassOf.h"
//#include "Templates/UnrealTemplate.h"
//#include "UObject/Class.h"
#include "UObject/UObjectGlobals.h"

#include "PPEquipmentManagerComponent.generated.h"

class UActorComponent;
class UPPAbilitySystemComponent;
class UPPEquipmentDefinition;
class UPPEquipmentInstance;
class UPPEquipmentManagerComponent;
class UObject;
struct FFrame;
struct FPPEquipmentList;
//struct FNetDeltaSerializeInfo;
//struct FReplicationFlags;

/** A single piece of applied equipment */
USTRUCT(BlueprintType)
struct FPPAppliedEquipmentEntry 
{
	GENERATED_BODY()

	FPPAppliedEquipmentEntry()
	{}

	FString GetDebugString() const;

private:
	friend FPPEquipmentList;
	friend UPPEquipmentManagerComponent;

	// The equipment class that got equipped
	UPROPERTY()
	TSubclassOf<UPPEquipmentDefinition> EquipmentDefinition;

	UPROPERTY()
	TObjectPtr<UPPEquipmentInstance> Instance = nullptr;

	// Authority-only list of granted handles
	UPROPERTY()
	FPPAbilitySet_GrantedHandles GrantedHandles;
};

/** List of applied equipment */
USTRUCT(BlueprintType)
struct FPPEquipmentList 
{
	GENERATED_BODY()

	FPPEquipmentList()
		: OwnerComponent(nullptr)
	{
	}

	FPPEquipmentList(UActorComponent* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

	UPPEquipmentInstance* AddEntry(TSubclassOf<UPPEquipmentDefinition> EquipmentDefinition);
	void RemoveEntry(UPPEquipmentInstance* Instance);

private:
	UPPAbilitySystemComponent* GetAbilitySystemComponent() const;

	friend UPPEquipmentManagerComponent;

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FPPAppliedEquipmentEntry> Entries;

	UPROPERTY(NotReplicated)
	TObjectPtr<UActorComponent> OwnerComponent;
};











/**
 * Manages equipment applied to a pawn
 */
UCLASS(BlueprintType, Const)
class UPPEquipmentManagerComponent : public UPawnComponent
{
	GENERATED_BODY()

public:
	UPPEquipmentManagerComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	UPPEquipmentInstance* EquipItem(TSubclassOf<UPPEquipmentDefinition> EquipmentDefinition);

	UFUNCTION(BlueprintCallable)
	void UnequipItem(UPPEquipmentInstance* ItemInstance);

	//~UActorComponent interface
	//virtual void EndPlay() override;
	virtual void InitializeComponent() override;
	virtual void UninitializeComponent() override;
	//~End of UActorComponent interface

	/** Returns the first equipped instance of a given type, or nullptr if none are found */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPPEquipmentInstance* GetFirstInstanceOfType(TSubclassOf<UPPEquipmentInstance> InstanceType);

 	/** Returns all equipped instances of a given type, or an empty array if none are found */
 	UFUNCTION(BlueprintCallable, BlueprintPure)
	TArray<UPPEquipmentInstance*> GetEquipmentInstancesOfType(TSubclassOf<UPPEquipmentInstance> InstanceType) const;

	template <typename T>
	T* GetFirstInstanceOfType()
	{
		return (T*)GetFirstInstanceOfType(T::StaticClass());
	}

private:
	UPROPERTY()
	FPPEquipmentList EquipmentList;
};
