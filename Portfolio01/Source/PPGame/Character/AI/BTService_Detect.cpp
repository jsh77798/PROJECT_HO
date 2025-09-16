// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/AI/BTService_Detect.h"

#include "PPAIController.h"
#include "Character/PPCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DrawDebugHelpers.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 1.0f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn) return;
	UWorld* World = ControllingPawn->GetWorld();
	FVector Center = ControllingPawn->GetActorLocation();
	float DetectRadius = 500.f;

	if (nullptr == World) return;
	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(NAME_None, false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		ECollisionChannel::ECC_GameTraceChannel2,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);
	//검색전 타겟은 초기화를 해줘야 없어졌을때 패트롤 모드로 간다. 아직도 복수의 타겟 처리가 안되었다.
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(APPAIController::TargetKey, nullptr);
	if (bResult)
	{  // 디텍팅된 플레이어가 없어졌을 경우의 처리가 없음
		for (auto const& OverlapResult : OverlapResults)
		{
			APPCharacter* Character = Cast<APPCharacter>(OverlapResult.GetActor());

			if (Character && Character->GetController()->IsPlayerController())
			{
				//DrawDebugString(GetWorld(), ABCharacter->GetActorLocation(), FString::Printf(TEXT("%s"), *ABCharacter->GetName()), 0, FColor::White, 1, false);
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(APPAIController::TargetKey, Character);
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.1f);

				DrawDebugPoint(World, Character->GetActorLocation(), 10.0f, FColor::Blue, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Character->GetActorLocation(), FColor::Blue, false, 1.0f);
				DrawDebugSphere(World, Character->GetActorLocation(), 60.f, 8, FColor::Green, false, 1.0f);
				return;
			}
		}
	}

	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.1f);
}
