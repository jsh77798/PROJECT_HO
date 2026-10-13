// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/AI/BTTask_MOVE.h"
#include <Global/Portfolio_GlobalCharacter.h>
#include <Game/AI/Portfolio_AIController.h>
#include <Game/AI/Portfolio_MonsterEnums.h>
#include <BehaviorTree/BlackboardComponent.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

// 일단 써치 범위 바깥으로 이동하면
// Return 상태로 전환하는 것은 2가지 상황이 존재한다.
// 1. 그냥 플레이어가 너무 멀어져서 나는 돌아간다.
// 2. 내 원래자리(만들어진 순간에 자신의 위치.)에서 너무 멀리 떨어졌다.


UBTTask_MOVE::UBTTask_MOVE()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MOVE::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto* Character = GetGlobalCharacter(OwnerComp);
	UBlackboardComponent* Blackboard =
		OwnerComp.GetBlackboardComponent();

	if (!IsValid(Character) || !IsValid(Blackboard))
	{
		return EBTNodeResult::Failed;
	}

	// ExecuteTask 전용 상태 변경.
	// 작업 종료는 아래 return 값으로 처리합니다.
	auto ChangeState = [&](AIState NewState)
		{
			Blackboard->SetValueAsEnum(
				TEXT("AIState"),
				static_cast<uint8>(NewState)
			);

			ResetStateTime(OwnerComp);
		};

	// 대상이 없어도 사망 판정부터 실행
	if (Character->GetHP() <= 0)
	{
		Character->GetCharacterMovement()->StopMovementImmediately();

		ChangeState(AIState::DEATH);
		return EBTNodeResult::Failed;
	}

	AActor* TargetActor = Cast<AActor>(
		Blackboard->GetValueAsObject(TEXT("TargetActor"))
	);

	if (!IsValid(TargetActor))
	{
		Character->GetCharacterMovement()->StopMovementImmediately();

		Blackboard->ClearValue(TEXT("TargetActor"));
		Blackboard->ClearValue(TEXT("NavPath"));

		ChangeState(AIState::RETURN);
		return EBTNodeResult::Failed;
	}

	const FVector TargetPos = TargetActor->GetActorLocation();

	// 이미 공격 거리 안이라면 경로를 만들 필요 없음
	const float AttackRange =
		Blackboard->GetValueAsFloat(TEXT("AttackRange"));

	const float Distance = FVector::Dist2D(
		Character->GetActorLocation(),
		TargetPos
	);

	if (Distance <= AttackRange)
	{
		Character->GetCharacterMovement()->StopMovementImmediately();

		ChangeState(AIState::ATTACK);
		return EBTNodeResult::Failed;
	}

	UNavigationPath* Path = PathFindNavPath(OwnerComp, TargetPos);

	if (!IsValid(Path) ||
		!Path->IsValid() ||
		Path->PathPoints.Num() < 2)
	{
		Character->GetCharacterMovement()->StopMovementImmediately();

		Blackboard->ClearValue(TEXT("NavPath"));

		ChangeState(AIState::RETURN);
		return EBTNodeResult::Failed;
	}

	Blackboard->SetValueAsObject(TEXT("NavPath"), Path);
	Blackboard->SetValueAsVector(TEXT("LastPos"), TargetPos);

	Character->GetCharacterMovement()->MaxWalkSpeed = GetGlobalCharacter(OwnerComp)->GetSpeed();
	Character->SetAniState(AIState::MOVE);

	return EBTNodeResult::InProgress;
}

void UBTTask_MOVE::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DelataSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DelataSeconds);

    auto* Character = GetGlobalCharacter(OwnerComp);
    UBlackboardComponent* Blackboard =
        OwnerComp.GetBlackboardComponent();

    if (!IsValid(Character) || !IsValid(Blackboard))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return;
    }

    UCharacterMovementComponent* MoveCom =
        Character->GetCharacterMovement();

    // 대상 유무와 관계없이 사망·스턴부터 검사.
    // 이 함수들은 내부에서 이미 SetStateChange()를 호출합니다.
    if (IsDeathCheck(OwnerComp))
    {
        MoveCom->StopMovementImmediately();
        return;
    }

    if (IsStunCheck(OwnerComp))
    {
        MoveCom->StopMovementImmediately();
        return;
    }

    // 실행 중인 작업에서 사용하는 상태 변경
    auto ChangeState = [&](AIState NewState)
        {
            MoveCom->StopMovementImmediately();
            SetStateChange(OwnerComp, NewState);
        };

    AActor* TargetActor = Cast<AActor>(
        Blackboard->GetValueAsObject(TEXT("TargetActor"))
    );

    if (!IsValid(TargetActor))
    {
        Blackboard->ClearValue(TEXT("TargetActor"));
        Blackboard->ClearValue(TEXT("NavPath"));

        ChangeState(AIState::RETURN);
        return;
    }

    const FVector ThisPos = Character->GetActorLocation();
    const FVector TargetPos = TargetActor->GetActorLocation();

    const FVector OriginPos =
        Blackboard->GetValueAsVector(TEXT("OriginPos"));

    const float SearchRange =
        Blackboard->GetValueAsFloat(TEXT("SearchRange"));

    const float AttackRange =
        Blackboard->GetValueAsFloat(TEXT("AttackRange"));

    const float TargetDistance =
        FVector::Dist2D(ThisPos, TargetPos);

    // 원래 위치 또는 추적 대상에게서 너무 멀어지면 복귀
    if (FVector::Dist2D(ThisPos, OriginPos) >= SearchRange * 1.5f ||
        TargetDistance > SearchRange)
    {
        ChangeState(AIState::RETURN);
        return;
    }

    // 경로 검사보다 먼저 공격 거리 확인
    if (TargetDistance <= AttackRange)
    {
        ChangeState(AIState::ATTACK);
        return;
    }

    UNavigationPath* NavPath = Cast<UNavigationPath>(
        Blackboard->GetValueAsObject(TEXT("NavPath"))
    );

    const FVector LastPos =
        Blackboard->GetValueAsVector(TEXT("LastPos"));

    // 경로가 없거나 대상이 일정 거리 이상 이동하면 갱신
    const bool bNeedNewPath =
        !IsValid(NavPath) ||
        !NavPath->IsValid() ||
        NavPath->PathPoints.Num() < 2 ||
        FVector::Dist2D(LastPos, TargetPos) >= 30.0f;

    if (bNeedNewPath)
    {
        NavPath = PathFindNavPath(OwnerComp, TargetPos);

        if (!IsValid(NavPath) ||
            !NavPath->IsValid() ||
            NavPath->PathPoints.Num() < 2)
        {
            Blackboard->ClearValue(TEXT("NavPath"));

            ChangeState(AIState::RETURN);
            return;
        }

        Blackboard->SetValueAsObject(TEXT("NavPath"), NavPath);

        // 갱신한 목표 위치도 반드시 저장
        Blackboard->SetValueAsVector(TEXT("LastPos"), TargetPos);
    }

    constexpr float WaypointAcceptanceRadius = 20.0f;

    // 다음 경로 지점에 도착했다면 지나간 지점을 제거.
    // [1]이 그다음 이동 지점을 가리키게 됩니다.
    while (NavPath->PathPoints.Num() > 2 &&
        FVector::Dist2D(
            ThisPos,
            NavPath->PathPoints[1]
        ) <= WaypointAcceptanceRadius)
    {
        NavPath->PathPoints.RemoveAt(0);
    }

    const FVector NextPoint = NavPath->PathPoints[1];

    // 경로 끝에 도달했지만 아직 공격할 수 없는 경우.
    // 부분 경로의 끝 등에서 제자리 걷기를 반복하지 않게 함.
    if (NavPath->PathPoints.Num() == 2 &&
        FVector::Dist2D(ThisPos, NextPoint) <= WaypointAcceptanceRadius)
    {
        Blackboard->ClearValue(TEXT("NavPath"));

        ChangeState(AIState::RETURN);
        return;
    }

    const FVector Direction =
        (NextPoint - ThisPos).GetSafeNormal2D();

    if (!Direction.IsNearlyZero())
    {
        const FRotator DesiredRotation = Direction.Rotation();

        Character->SetActorRotation(
            FMath::RInterpTo(
                Character->GetActorRotation(),
                DesiredRotation,
                DelataSeconds,
                8.0f
            )
        );

        Character->AddMovementInput(Direction);
    }
}
