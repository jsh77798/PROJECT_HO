// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"

#include "BTDecorator_MeleeAttackRange.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API UBTDecorator_MeleeAttackRange : public UBTDecorator
{
	GENERATED_BODY()

public:
	UBTDecorator_MeleeAttackRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

private:
	UPROPERTY(Category = "Range", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	float MeleeAttackRange = 200.0f;
};
