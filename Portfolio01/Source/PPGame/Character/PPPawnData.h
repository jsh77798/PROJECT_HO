// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Containers/Array.h"
#include "Engine/DataAsset.h"
#include "Templates/SubclassOf.h"
#include "UObject/UObjectGlobals.h"

#include "PPPawnData.generated.h"

class APawn;
class UPPAbilitySet;
class UPPAbilityTagRelationshipMapping;
class UPPCameraMode;
class UPPInputConfig;
class UObject;


/**
 * UPPPawnData
 *
 *	Non-mutable data asset that contains properties used to define a pawn.
 */
UCLASS(BlueprintType, Const, Meta = (DisplayName = "PP Pawn Data", ShortTooltip = "Data asset used to define a Pawn."))
class PPGAME_API UPPPawnData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPPPawnData(const FObjectInitializer& ObjectInitializer);

public:

	// Class to instantiate for this pawn (should usually derive from APPPawn or APPCharacter).
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PP|Pawn")
	TSubclassOf<APawn> PawnClass;

	// Ability sets to grant to this pawn's ability system.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PP|Abilities")
	TArray<TObjectPtr<UPPAbilitySet>> AbilitySets;

	// What mapping of ability tags to use for actions taking by this pawn
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PP|Abilities")
	TObjectPtr<UPPAbilityTagRelationshipMapping> TagRelationshipMapping;

	// Input configuration used by player controlled pawns to create input mappings and bind input actions.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PP|Input")
	TObjectPtr<UPPInputConfig> InputConfig;

	// Default camera mode used by player controlled pawns.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "PP|Camera")
	TSubclassOf<UPPCameraMode> DefaultCameraMode;
};
