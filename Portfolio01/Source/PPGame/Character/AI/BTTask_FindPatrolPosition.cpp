// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPatrolPosition.h"

#include "PPAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_FindPatrolPosition::UBTTask_FindPatrolPosition()
{
	NodeName = TEXT("FindPatrolPosition");
}


EBTNodeResult::Type UBTTask_FindPatrolPosition::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto AIControllerPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (AIControllerPawn == nullptr) {return EBTNodeResult::Type::Failed;}

	UNavigationSystemV1* NavigationSystem = UNavigationSystemV1::GetNavigationSystem(AIControllerPawn->GetWorld());
	if (NavigationSystem == nullptr) { return EBTNodeResult::Type::Failed; }

	FVector OriginPos = OwnerComp.GetBlackboardComponent()->GetValueAsVector(APPAIController::OriginPosKey);
	FNavLocation NextPos;

	if (NavigationSystem->GetRandomPointInNavigableRadius(OriginPos, 500.f, NextPos))
	{
		OwnerComp.GetBlackboardComponent()->SetValueAsVector(APPAIController::PatrolPosKey, NextPos.Location);
		return EBTNodeResult::Type::Succeeded;
	}

	return EBTNodeResult::Type::Failed;
}
