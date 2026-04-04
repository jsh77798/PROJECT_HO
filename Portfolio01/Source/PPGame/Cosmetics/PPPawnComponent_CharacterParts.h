// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Components/PawnComponent.h"
#include "Containers/Array.h"
#include "Containers/ArrayView.h"
#include "Containers/Map.h"
#include "Containers/Set.h"
#include "Containers/SparseArray.h"
#include "Containers/UnrealString.h"
#include "CoreTypes.h"
#include "PPCosmeticAnimationTypes.h"
#include "Delegates/Delegate.h"
#include "Engine/EngineTypes.h"
#include "GameplayTagContainer.h"
#include "PPCharacterPartTypes.h"
#include "Templates/UnrealTemplate.h"
#include "UObject/Class.h"
#include "UObject/ObjectPtr.h"
#include "UObject/UObjectGlobals.h"

#include "PPPawnComponent_CharacterParts.generated.h"

class AActor;
class UChildActorComponent;
class UObject;
class USceneComponent;
class USkeletalMeshComponent;
struct FFrame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPPSpawnedCharacterPartsChanged, UPPPawnComponent_CharacterParts*, ComponentWithChangedParts);

//////////////////////////////////////////////////////////////////////

// A single applied character part
USTRUCT()
struct FPPAppliedCharacterPartEntry 
{
	GENERATED_BODY()

	FString GetDebugString() const;

private:
	friend FPPCharacterPartList;
	friend UPPPawnComponent_CharacterParts;

private:
	// The character part being represented
	UPROPERTY()
	FPPCharacterPart Part;

	// Handle index we returned to the user (server only)
	UPROPERTY()
	int32 PartHandle = INDEX_NONE;

	// The spawned actor instance (client only)
	UPROPERTY()
	TObjectPtr<UChildActorComponent> SpawnedComponent = nullptr;
};

//////////////////////////////////////////////////////////////////////

// Replicated list of applied character parts
USTRUCT(BlueprintType)
struct FPPCharacterPartList 
{
	GENERATED_BODY()

	FPPCharacterPartList()
		: OwnerComponent(nullptr)
	{
	}

	FPPCharacterPartList(UPPPawnComponent_CharacterParts* InOwnerComponent)
		: OwnerComponent(InOwnerComponent)
	{
	}

public:
	FPPCharacterPartHandle AddEntry(FPPCharacterPart NewPart);
	void RemoveEntry(FPPCharacterPartHandle Handle);
	void ClearAllEntries(bool bBroadcastChangeDelegate);

	FGameplayTagContainer CollectCombinedTags() const;
private:
	friend UPPPawnComponent_CharacterParts;

	bool SpawnActorForEntry(FPPAppliedCharacterPartEntry& Entry);
	bool DestroyActorForEntry(FPPAppliedCharacterPartEntry& Entry);

private:
	// Replicated list of equipment entries
	UPROPERTY()
	TArray<FPPAppliedCharacterPartEntry> Entries;

	// The component that contains this list
	UPROPERTY()
	TObjectPtr<UPPPawnComponent_CharacterParts> OwnerComponent;

	// Upcounter for handles
	int32 PartHandleCounter = 0;
};

//////////////////////////////////////////////////////////////////////

// A component that handles spawning cosmetic actors attached to the owner pawn on all clients
UCLASS(meta=(BlueprintSpawnableComponent))
class UPPPawnComponent_CharacterParts : public UPawnComponent
{
	GENERATED_BODY()

public:
	UPPPawnComponent_CharacterParts(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	//~UActorComponent interface
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~End of UActorComponent interface

	// Adds a character part to the actor that owns this customization component, should be called on the authority only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Cosmetics)
	FPPCharacterPartHandle AddCharacterPart(const FPPCharacterPart& NewPart);

	// Removes a previously added character part from the actor that owns this customization component, should be called on the authority only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Cosmetics)
	void RemoveCharacterPart(FPPCharacterPartHandle Handle);

	// Removes all added character parts, should be called on the authority only
	UFUNCTION(BlueprintCallable, BlueprintAuthorityOnly, Category=Cosmetics)
	void RemoveAllCharacterParts();

	// Gets the list of all spawned character parts from this component
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Cosmetics)
	TArray<AActor*> GetCharacterPartActors() const;

	// If the parent actor is derived from ACharacter, returns the Mesh component, otherwise nullptr
	USkeletalMeshComponent* GetParentMeshComponent() const;

	// Returns the scene component to attach the spawned actors to
	// If the parent actor is derived from ACharacter, we'll use the Mesh component, otherwise the root component
	USceneComponent* GetSceneComponentToAttachTo() const;

	// Returns the set of combined gameplay tags from attached character parts, optionally filtered to only tags that start with the specified root
	UFUNCTION(BlueprintCallable, BlueprintPure=false, Category=Cosmetics)
	FGameplayTagContainer GetCombinedTags(FGameplayTag RequiredPrefix) const;

	void BroadcastChanged();

public:
	// Delegate that will be called when the list of spawned character parts has changed
	UPROPERTY(BlueprintAssignable, Category=Cosmetics, BlueprintCallable)
	FPPSpawnedCharacterPartsChanged OnCharacterPartsChanged;

private:
	// List of character parts
	UPROPERTY()
	FPPCharacterPartList CharacterPartList;

	// Rules for how to pick a body style mesh for animation to play on, based on character part cosmetics tags
	UPROPERTY(EditAnywhere, Category=Cosmetics)
	FPPAnimBodyStyleSelectionSet BodyMeshes;
};
