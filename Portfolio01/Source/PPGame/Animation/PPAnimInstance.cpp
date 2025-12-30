// Copyright Epic Games, Inc. All Rights Reserved.

#include "PPAnimInstance.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Character/PPCharacter.h"
#include "Character/PPCharacterMovementComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPAnimInstance)


UPPAnimInstance::UPPAnimInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UPPAnimInstance::InitializeWithAbilitySystem(UAbilitySystemComponent* ASC)
{
	check(ASC);

	GameplayTagPropertyMap.Initialize(this, ASC);
}

#if WITH_EDITOR
EDataValidationResult UPPAnimInstance::IsDataValid(TArray<FText>& ValidationErrors)
{
	Super::IsDataValid(ValidationErrors);

	GameplayTagPropertyMap.IsDataValid(this, ValidationErrors);

	return ((ValidationErrors.Num() > 0) ? EDataValidationResult::Invalid : EDataValidationResult::Valid);
}
#endif // WITH_EDITOR

void UPPAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	if (AActor* OwningActor = GetOwningActor())
	{
		if (UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor))
		{
			InitializeWithAbilitySystem(ASC);
		}
	}
}

void UPPAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	const APPCharacter* Character = Cast<APPCharacter>(GetOwningActor());
	if (!Character)
	{
		return;
	}

	UPPCharacterMovementComponent* CharMoveComp = CastChecked<UPPCharacterMovementComponent>(Character->GetCharacterMovement());
	const FPPCharacterGroundInfo& GroundInfo = CharMoveComp->GetGroundInfo();
	GroundDistance = GroundInfo.GroundDistance;
}

bool UPPAnimInstance::ShouldEnableControlRig()
{
	const bool bLegIKAllowed = (GetCurveValue(FName(TEXT("DisableLegIK"))) <= 0.0f);

	const bool bUseFootPlacement = !UseFootPlacement;

	return bLegIKAllowed && bUseFootPlacement;
}

UCharacterMovementComponent* UPPAnimInstance::GetMovementComponent()
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

AnimEnum_CardinalDirection UPPAnimInstance::SelectCardinalDirectionfromAngle(float Angle, float DeadZone, AnimEnum_CardinalDirection CurrentDirection, bool UseCurrentDirection)
{
	float AbsAngle = UKismetMathLibrary::Abs(Angle);

	float FwdDeadZone = DeadZone;

	float BwdDeadZone = FwdDeadZone;

	{
		if (UseCurrentDirection)
		{
			switch (CurrentDirection)
			{
			case AnimEnum_CardinalDirection::Forward:
				FwdDeadZone *= 2.0f;
				break;
			case AnimEnum_CardinalDirection::Backward:
				BwdDeadZone *= 2.0f;
				break;
			}
		}
	}

	{
		if (AbsAngle <= (45.0+ FwdDeadZone))
		{
			return AnimEnum_CardinalDirection::Forward;
		}
		else if (AbsAngle >= (135.0f - BwdDeadZone))
		{
			return AnimEnum_CardinalDirection::Backward;
		}
		else if (Angle > 0.0f)
		{
			return AnimEnum_CardinalDirection::Right;
		}
		else
		{
			return AnimEnum_CardinalDirection::Left;
		}
	}
}

AnimEnum_CardinalDirection UPPAnimInstance::GetOppositeCardinalDirection(AnimEnum_CardinalDirection CurrentDirection) const
{
	switch (CurrentDirection)
	{
	case AnimEnum_CardinalDirection::Forward:
		return AnimEnum_CardinalDirection::Backward;
	case AnimEnum_CardinalDirection::Backward:
		return AnimEnum_CardinalDirection::Forward;
	case AnimEnum_CardinalDirection::Left:
		return AnimEnum_CardinalDirection::Right;
	case AnimEnum_CardinalDirection::Right:
		return AnimEnum_CardinalDirection::Left;
	default:
		return CurrentDirection;
	}
}

bool UPPAnimInstance::IsMovingPerpendicularToInitialPivot()
{
	if ((PivotInitialDirection == AnimEnum_CardinalDirection::Forward || PivotInitialDirection == AnimEnum_CardinalDirection::Backward) &&
		!(LocalVelocityDirection == AnimEnum_CardinalDirection::Forward || LocalVelocityDirection == AnimEnum_CardinalDirection::Backward))
	{
		return true;
	}
	else if ((PivotInitialDirection == AnimEnum_CardinalDirection::Left || PivotInitialDirection == AnimEnum_CardinalDirection::Right) &&
		!(LocalVelocityDirection == AnimEnum_CardinalDirection::Left || LocalVelocityDirection == AnimEnum_CardinalDirection::Right))
	{
		return true;
	}
	
	return false;
}

void UPPAnimInstance::UpdateLocationData(float DeltaTime)
{
	FVector ActorLocation = GetOwningActor()->GetActorLocation();

	DisplacementSinceLastUpdate = UKismetMathLibrary::VSizeXY(ActorLocation - WorldLocation);

	WorldLocation = ActorLocation;

	DisplacementSpeed = UKismetMathLibrary::SafeDivide(DisplacementSinceLastUpdate, DeltaTime);

	if (IsFirstUpdate)
	{
		DisplacementSinceLastUpdate = 0.0f;
		DisplacementSpeed = 0.0f;
	}
}

void UPPAnimInstance::UpdateRotationData()
{
	FRotator ActorRotation = GetOwningActor()->GetActorRotation();

	YawDeltaSinceLastUpdate = ActorRotation.Yaw - WorldRotation.Yaw;

	YawDeltaSpeed = UKismetMathLibrary::SafeDivide(YawDeltaSinceLastUpdate, GetWorld()->GetDeltaSeconds());

	WorldRotation = ActorRotation;

	AdditiveLeanAngle = YawDeltaSpeed * UKismetMathLibrary::SelectFloat(0.025f, 0.0375f, IsCrouching || GameplayTag_IsADS);

	if (IsFirstUpdate)
	{
		YawDeltaSinceLastUpdate = 0.0f;
		AdditiveLeanAngle = 0.0f;
	}
}

void UPPAnimInstance::UpdateVelocityData()
{
	bool WasMovingLastUpdate = !UKismetMathLibrary::Vector_IsZero(LocalVelocity2D);

	WorldVelocity = TryGetPawnOwner()->GetVelocity();

	WorldVelocity.Z *= 0.f;
	FVector WorldVelocity2D = WorldVelocity;

	LocalVelocity2D = UKismetMathLibrary::LessLess_VectorRotator(WorldVelocity2D, WorldRotation);

	LocalVelocityDirectionAngle = CalculateDirection(WorldVelocity2D, WorldRotation);

	LocalVelocityDirectionAngleWithOffset = LocalVelocityDirectionAngle - RootYawOffset;

	LocalVelocityDirection = SelectCardinalDirectionfromAngle(LocalVelocityDirectionAngleWithOffset, CardinalDirectionDeadZone, LocalVelocityDirection, WasMovingLastUpdate);
	
	LocalVelocityDirectionNoOffset = SelectCardinalDirectionfromAngle(LocalVelocityDirectionAngle, CardinalDirectionDeadZone, LocalVelocityDirectionNoOffset, WasMovingLastUpdate);

	HasVelocity = !UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::VSizeXYSquared(LocalVelocity2D), 0.0, 0.000001);
}

void UPPAnimInstance::UpdateAccelerationData()
{
}

void UPPAnimInstance::UpdateCharacterStateData(float DeltaTime)
{
}

void UPPAnimInstance::UpdateBlendWeightData(float DeltaTime)
{
}

void UPPAnimInstance::UpdateAimingData()
{
}

void UPPAnimInstance::UpdateWallDetectionHeuristic()
{
}


