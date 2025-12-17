// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PPAnimLayerInstance.h"

#include "Kismet/KismetMathLibrary.h"
#include "PPAnimInstance.h"
#include "Character/PPCharacter.h"
#include "Character/PPCharacterMovementComponent.h"

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
}

void UPPAnimLayerInstance::UpdateSkelControlData()
{
	float Result = UKismetMathLibrary::SelectFloat(0.0f, 1.0f, DisableHandIK);

	HandIK_Right_Alpha = UKismetMathLibrary::FClamp(Result - GetCurveValue(TEXT("DisableRHandIK")), 0.0f, 1.0f);

	HandIK_Left_Alpha = UKismetMathLibrary::FClamp(Result - GetCurveValue(TEXT("DisableLHandIK")), 0.0f, 1.0f);
}
