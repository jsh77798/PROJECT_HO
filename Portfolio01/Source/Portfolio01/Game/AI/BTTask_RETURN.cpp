// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/BTTask_RETURN.h"
#include <Global/Portfolio_GlobalCharacter.h>
#include <Global/Portfolio_Global.h>
#include <Game/AI/Portfolio_MonsterEnums.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "BTTask_IDLE.h"
#include "Portfolio_AIMonster.h"
#include "GameFramework/CharacterMovementComponent.h"


UBTTask_RETURN::UBTTask_RETURN()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}


EBTNodeResult::Type UBTTask_RETURN::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Character = GetGlobalCharacter(OwnerComp);
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!IsValid(Character) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	// 사망 상태를 RETURN 애니메이션으로 덮어쓰지 않음
	if (Character->GetHP() <= 0)
	{
		Character->GetCharacterMovement()->StopMovementImmediately();

		Blackboard->SetValueAsEnum(
			TEXT("AIState"),
			static_cast<uint8>(AIState::DEATH)
		);

		ResetStateTime(OwnerComp);
		return EBTNodeResult::Failed;
	}

	GetGlobalCharacter(OwnerComp)->SetAniState(AIState::RETURN);

	UCharacterMovementComponent* MoveCom = Cast<UCharacterMovementComponent>(GetGlobalCharacter(OwnerComp)->GetMovementComponent());

	if (nullptr != MoveCom)
	{
		MoveCom->MaxWalkSpeed = GetGlobalCharacter(OwnerComp)->GetSpeed() * 0.8f; // 80%
	}

	FVector OriginPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("OriginPos"));
	UNavigationPath* PathPoint = PathFindNavPath(OwnerComp, OriginPos);
	GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("NavPath"), PathPoint);
	GetBlackboardComponent(OwnerComp)->SetValueAsInt(TEXT("CurrentIndex"), 1);

	return EBTNodeResult::Type::InProgress;
}


void UBTTask_RETURN::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{

	auto* Character = GetGlobalCharacter(OwnerComp);
	UBlackboardComponent* Blackboard =
		OwnerComp.GetBlackboardComponent();

	if (!IsValid(Character) || !IsValid(Blackboard))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);

	// 내부에서 이미 상태 변경 및 작업 종료를 수행함
	if (IsDeathCheck(OwnerComp))
	{
		return;
	}

	if (IsStunCheck(OwnerComp))
	{
		return;
	}

	UNavigationPath* NavPath = Cast<UNavigationPath>(
		Blackboard->GetValueAsObject(TEXT("NavPath"))
	);

	if (!IsValid(NavPath) ||
		!NavPath->IsValid() ||
		NavPath->PathPoints.Num() < 2)
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
		SetStateChange(OwnerComp, AIState::IDLE);
		return;
	}

	int CurrentIndex =
		Blackboard->GetValueAsInt(TEXT("CurrentIndex"));

	if (!NavPath->PathPoints.IsValidIndex(CurrentIndex))
	{
		Character->GetCharacterMovement()->StopMovementImmediately();
		SetStateChange(OwnerComp, AIState::IDLE);
		return;
	}

	FVector TargetPos = NavPath->PathPoints[CurrentIndex];
	FVector ThisPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

	TargetPos.Z = 0.0f;
	ThisPos.Z = 0.0f;

	{
		// 혹시라도 z축이 있을 가능성을 없애는게 보통입니다.

		FVector Dir = TargetPos - ThisPos;
		Dir.Normalize();

		FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (FMath::Abs(Angle0 - Angle1) >= 10.0f)
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.0f * DelataSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else {
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}

	{
		const FVector Dir = TargetPos - ThisPos;

		if (Dir.SizeSquared() <= FMath::Square(10.0f))
		{
			++CurrentIndex;

			if (CurrentIndex >= NavPath->PathPoints.Num())
			{
				Character->GetCharacterMovement()->StopMovementImmediately();
				SetStateChange(OwnerComp, AIState::IDLE);
				return;
			}

			Blackboard->SetValueAsInt(TEXT("CurrentIndex"), CurrentIndex);
			return;
		}

		Character->AddMovementInput(Dir.GetSafeNormal());
	}

}

