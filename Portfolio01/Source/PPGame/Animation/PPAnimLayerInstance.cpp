// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimLayerInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "PPAnimInstance.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Character/PPCharacter.h"
#include "Character/PPCharacterMovementComponent.h"
#include "AnimGraphRuntime/Public/AnimationStateMachineLibrary.h"
#include "AnimGraphRuntime/Public/SequencePlayerLibrary.h"
//#include "Animation/AnimNode_SequencePlayer.h"
#include "Animation/AnimExecutionContext.h"

UPPAnimInstance* UPPAnimLayerInstance::GetMainAnimThreadSafe() const
{
	UPPAnimInstance* AnimInstance = Cast<UPPAnimInstance>(GetOwningComponent()->GetAnimInstance());
	if (!AnimInstance)
	{
		return nullptr;
	}
	return AnimInstance;

}

bool UPPAnimLayerInstance::ShouldEnableFootPlacement() const
{
	UPPAnimInstance* PPAnimInstance = GetMainAnimThreadSafe();
    if (!PPAnimInstance)
    {
        return false;
    }

	const bool bLegIKAllowed = (GetCurveValue(FName(TEXT("DisableLegIK"))) <= 0.0f);

	const bool bUseFootPlacement = PPAnimInstance->UseFootPlacement;

    return bLegIKAllowed && bUseFootPlacement;
}

UCharacterMovementComponent* UPPAnimLayerInstance::GetMovementComponent() const
{
	APawn* PawnOwner = TryGetPawnOwner();
	if (!PawnOwner)
	{
		return nullptr;
	}

	ACharacter* Character = Cast<ACharacter>(PawnOwner);
	if (!Character)
	{
		return nullptr;
	}

	return Character->GetCharacterMovement();
}

void UPPAnimLayerInstance::UpdateBlendWeightData(float DeltaTime)
{
	AActor* OwningActor = GetOwningActor();

	UPPAnimInstance* AnimInstance = GetMainAnimThreadSafe();

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);

	bool bIsADS = (ASC && ASC->HasMatchingGameplayTag(AnimInstance->ADS_Tag));

	if (!AnimInstance)
	{
		return;
	}

	if ((!RaiseWeaponAfterFiringWhenCrouched && AnimInstance->IsCrouching) || ((!AnimInstance->IsCrouching && bIsADS) && AnimInstance->IsOnGround))
	{
		HipFireUpperBodyOverrideWeight = 0.f;
		AimOffsetBlendWeight = 1.f;
	}
	else 
	{
		if ((AnimInstance->TimeSinceFiredWeapon < RaiseWeaponAfterFiringDuration) || (bIsADS && (AnimInstance->IsCrouching || !AnimInstance->IsOnGround)) || (GetCurveValue("applyHipfireOverridePose") > 0.f))
		{
			HipFireUpperBodyOverrideWeight = 1.0f;
			AimOffsetBlendWeight = 1.0f;
		}
		else 
		{
			HipFireUpperBodyOverrideWeight = UKismetMathLibrary::FInterpTo(HipFireUpperBodyOverrideWeight, 0.0f, DeltaTime, 1.0f);
			double Target = UKismetMathLibrary::SelectFloat(HipFireUpperBodyOverrideWeight, 1.0f, FMath::Abs(AnimInstance->RootYawOffset) < 10.0f && AnimInstance->HasAcceleration);

			AimOffsetBlendWeight = UKismetMathLibrary::FInterpTo(AimOffsetBlendWeight, Target, DeltaTime, 10.0f);
		}
	}
}

void UPPAnimLayerInstance::UpdateSkelControlData()
{
	float Result = UKismetMathLibrary::SelectFloat(0.0f, 1.0f, DisableHandIK);

	HandIK_Right_Alpha = UKismetMathLibrary::FClamp(Result - GetCurveValue(TEXT("DisableRHandIK")), 0.0f, 1.0f);

	HandIK_Left_Alpha = UKismetMathLibrary::FClamp(Result - GetCurveValue(TEXT("DisableLHandIK")), 0.0f, 1.0f);
}
