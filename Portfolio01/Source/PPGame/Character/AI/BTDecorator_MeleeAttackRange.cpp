// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTDecorator_MeleeAttackRange.h"

#include "PPAIController.h"
#include "PPGame/Portfolio_Character.h"
#include "BehaviorTree/BlackboardComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_MeleeAttackRange)

UBTDecorator_MeleeAttackRange::UBTDecorator_MeleeAttackRange()
{
}

bool UBTDecorator_MeleeAttackRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool bResult = Super::CalculateRawConditionValue(OwnerComp, NodeMemory);

	auto ControllerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllerPawn == nullptr) return false;

	auto Target = Cast<APortfolio_Character>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(APPAIController::TargetKey));
	if (Target == nullptr) return false;
	
	bResult = (Target->GetDistanceTo(ControllerPawn) <= MeleeAttackRange);
	return bResult;
}
