// Fill out your copyright notice in the Description page of Project Settings.


#include "Global/Portfolio_GlobalAnimInstance.h"
#include "Global/Portfolio_GlobalCharacter.h"
#include <Global/Data/MonsterData.h>
#include "Portfolio_Global.h"
#include "Game/Portfolio_Enums.h"

void UPortfolio_GlobalAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
	OnMontageBlendingOut.AddDynamic(this, &UPortfolio_GlobalAnimInstance::MontageEnd);
}

// 시작할때 한번 업데이트를 돌려주므로.
void UPortfolio_GlobalAnimInstance::NativeUpdateAnimation(float _DeltaTime)
{
    Super::NativeUpdateAnimation(_DeltaTime);

    APortfolio_GlobalCharacter* Character =
        Cast<APortfolio_GlobalCharacter>(GetOwningActor());

    if (!IsValid(Character))
    {
        MoveSpeed = 0.0f;
        MoveDirection = 0.0f;
        bUseUpperBody = false;
        return;
    }

    // 공격·장전 상태와 관계없이 이동 정보 갱신
    const FVector Velocity = Character->GetVelocity();
    MoveSpeed = Velocity.Size2D();

    const FRotator YawRotation(
        0.0f, Character->GetActorRotation().Yaw, 0.0f);

    const FVector LocalVelocity =
        YawRotation.UnrotateVector(Velocity);

    MoveDirection = MoveSpeed > 1.0f
        ? FMath::RadiansToDegrees(
            FMath::Atan2(LocalVelocity.Y, LocalVelocity.X))
        : 0.0f;

    AniState = Character->GetAniState();

    // 플레이어만 상체 분리 적용
    // 적이 같은 AnimInstance 클래스를 사용해도 영향 없도록 구분
    bUseUpperBody =
        Character->IsPlayerControlled() &&
        (AniState == static_cast<int>(EAniState::W_Attack) ||
            AniState == static_cast<int>(EAniState::Load));

    UAnimMontage* const* FoundMontage =
        AllAnimations.Find(AniState);

    if (!FoundMontage || !IsValid(*FoundMontage))
    {
        return;
    }

    UAnimMontage* Montage = *FoundMontage;

    if (!Montage_IsPlaying(Montage))
    {
        CurMontage = Montage;
        Montage_Play(Montage, 1.0f);
    }
}

void UPortfolio_GlobalAnimInstance::MontageEnd(UAnimMontage* Anim, bool _Inter)
{

}
