#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Abilities/GameplayAbility.h"
#include <Game/Character/CharacterStateEnums.h>

#include "AbilityData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct PORTFOLIO01_API FAbilityData : public FTableRowBase
{
	GENERATED_BODY()

	/** Ability */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ability") 
		TMap<FName, TSubclassOf<UGameplayAbility> > Ability;

};
