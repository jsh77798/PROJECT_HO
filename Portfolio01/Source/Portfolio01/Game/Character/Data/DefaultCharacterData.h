#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"

#include "DefaultCharacterData.generated.h"

/**
 *
 */
USTRUCT(BlueprintType)
struct PORTFOLIO01_API FDefaultCharacterData : public FTableRowBase
{

	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //HP설정
	    int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data") //AI설정
		class UBehaviorTree* AI;

};
