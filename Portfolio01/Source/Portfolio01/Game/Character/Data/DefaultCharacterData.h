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

	/** AI */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI") 
		class UBehaviorTree* AI;

	/** Status */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status") 
	    int Health = 0;

	/** Movement */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") 
		bool bOrientRotationToMovement = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement") 
		FRotator RotationRate = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxWalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float MinAnalogWalkSpeed = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
		float BrakingDecelerationWalking = 0.f;

};
