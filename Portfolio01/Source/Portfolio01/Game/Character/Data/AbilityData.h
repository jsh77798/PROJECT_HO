#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "Game/PPGameMode.h"
#include <Game/Character/CharacterStateEnums.h>

#include "AbilityData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct PORTFOLIO01_API FAbilityData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //¾îºô¸®Æ¼Å¸ÀÔ
		FName TypeName; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //¾îºô¸®Æ¼
		TMap<FName, class UGameplayAbility*> Ability;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //¾îºô¸®Æ¼Å¸ÀÔ
	//	TMap<EnumType, class UGameplayAbility*> Type;
	
	//enum class &Type = EnumTypeMap[TypeName]; // EnumTypeMap is a TMap<FName, EnumType> defined in PPGameMode

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //¾îºô¸®Æ¼
	//	TMap<Type, class UGameplayAbility*> Ability;

};
