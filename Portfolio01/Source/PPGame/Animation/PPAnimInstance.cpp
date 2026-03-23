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

	// Pawn Ä³½Ì
	APawn* Pawn = TryGetPawnOwner();
	if (!IsValid(Pawn))
	{
		CachedPawn.Reset();
		CachedCharacter.Reset();
		return;
	}
	CachedPawn = Pawn;

	// Character Ä³½Ì
	ACharacter* Character = Cast<ACharacter>(Pawn);
	if (!IsValid(Character))
	{
		CachedCharacter.Reset();
		return;
	}
	CachedCharacter = Character;

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
	APawn* PawnOwner = CachedPawn.Get();
	if (!PawnOwner)
	{
		return nullptr;
	}

	ACharacter* Character = CachedCharacter.Get();
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
	AActor* OwningActor = GetOwningActor();
	if (!IsValid(OwningActor))
	{
		DisplacementSinceLastUpdate = 0.f;
		DisplacementSpeed = 0.f;
		return;
	}

	FVector ActorLocation = OwningActor->GetActorLocation();

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
	AActor* OwningActor = GetOwningActor();

	UWorld* World = GetWorld();

	if (!IsValid(OwningActor) || !IsValid(World))
	{
		YawDeltaSinceLastUpdate = 0.f;
		YawDeltaSpeed = 0.f;
		AdditiveLeanAngle = 0.f;
		return;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);

	bool bIsADS = (ASC && ASC->HasMatchingGameplayTag(ADS_Tag));

	FRotator ActorRotation = OwningActor->GetActorRotation();

	YawDeltaSinceLastUpdate = ActorRotation.Yaw - WorldRotation.Yaw;

	YawDeltaSpeed = UKismetMathLibrary::SafeDivide(YawDeltaSinceLastUpdate, World->GetDeltaSeconds());

	WorldRotation = ActorRotation;

	AdditiveLeanAngle = YawDeltaSpeed * UKismetMathLibrary::SelectFloat(0.025f, 0.0375f, IsCrouching || bIsADS);

	if (IsFirstUpdate)
	{
		YawDeltaSinceLastUpdate = 0.0f;
		AdditiveLeanAngle = 0.0f;
	}
}

void UPPAnimInstance::UpdateVelocityData()
{
	bool WasMovingLastUpdate = !UKismetMathLibrary::Vector_IsZero(LocalVelocity2D);

	APawn* PawnOwner = CachedPawn.Get();
	if (!IsValid(PawnOwner))
	{
		WorldVelocity = FVector::ZeroVector;
		LocalVelocity2D = FVector::ZeroVector;
		HasVelocity = false;
		return;
	}

	WorldVelocity = PawnOwner->GetVelocity();
	WorldVelocity.Z = 0.f;

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
	UCharacterMovementComponent* MoveComp = GetMovementComponent();
	if (!IsValid(MoveComp))
	{
		LocalAcceleration2D = FVector::ZeroVector;
		HasAcceleration = false;
		return;
	}

	FVector WorldAcceleration2D = MoveComp->GetCurrentAcceleration();
	WorldAcceleration2D.Z *= 0.0f;

	LocalAcceleration2D = UKismetMathLibrary::LessLess_VectorRotator(WorldAcceleration2D, WorldRotation);

	HasAcceleration = !UKismetMathLibrary::NearlyEqual_FloatFloat(UKismetMathLibrary::VSizeXYSquared(LocalAcceleration2D), 0.0, 0.000001);

	PivotDirection2D = UKismetMathLibrary::Normal(UKismetMathLibrary::VLerp(PivotDirection2D, UKismetMathLibrary::Normal(WorldAcceleration2D, 0.0001f), 0.5f), 0.0001f);

	CardinalDirectionFromAcceleration = GetOppositeCardinalDirection(SelectCardinalDirectionfromAngle(UKismetAnimationLibrary::CalculateDirection(PivotDirection2D, WorldRotation), CardinalDirectionDeadZone, AnimEnum_CardinalDirection::Forward, false));
}

void UPPAnimInstance::UpdateCharacterStateData(float DeltaTime)
{
	AActor* OwningActor = GetOwningActor();

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwningActor);

	bool bIsADS = (ASC && ASC->HasMatchingGameplayTag(ADS_Tag));

	// ADS state
	{
		ADSStateChanged = (bIsADS != WasADSLastUpdate);

		WasADSLastUpdate = bIsADS;
	}

	// Weapon fired state
	{
		if (GameplayTag_IsFiring) 
		{
			TimeSinceFiredWeapon = 0.0f;
		}
		else
		{
			TimeSinceFiredWeapon += DeltaTime;
		}
	}

	{
		UCharacterMovementComponent* MoveComp = GetMovementComponent();
		if (!IsValid(MoveComp))
		{
			IsOnGround = false;
			IsCrouching = false;
			CrouchStateChange = false;
			return;
		}

		// On ground state
		{
			IsOnGround = MoveComp->IsMovingOnGround();
		}

		// Crouch state
		{
			bool WasCrouchingLastUpdate = IsCrouching;

			IsCrouching = MoveComp->IsCrouching();

			CrouchStateChange = (IsCrouching != WasCrouchingLastUpdate);
		}
	}
}

void UPPAnimInstance::UpdateBlendWeightData(float DeltaTime)
{
	UpperbodyDynamicAdditiveWeight = UKismetMathLibrary::SelectFloat(1.0, UKismetMathLibrary::FInterpTo(UpperbodyDynamicAdditiveWeight, 0.0, DeltaTime, 6.0), IsAnyMontagePlaying() && IsOnGround);
}

void UPPAnimInstance::UpdateAimingData()
{
	APawn* PawnOwner = CachedPawn.Get();
	if (!PawnOwner)
	{
		AimPitch = 0.f;
		return;
	}

	AimPitch = UKismetMathLibrary::NormalizeAxis(
		PawnOwner->GetBaseAimRotation().Pitch
	);
}

void UPPAnimInstance::UpdateWallDetectionHeuristic()
{
	IsRunningIntoWall =
	UKismetMathLibrary::VSizeXY(LocalAcceleration2D) > 0.1f &&
	UKismetMathLibrary::VSizeXY(LocalVelocity2D) < 200.0f &&
	UKismetMathLibrary::InRange_FloatFloat(UKismetMathLibrary::Dot_VectorVector(UKismetMathLibrary::Normal(LocalAcceleration2D), UKismetMathLibrary::Normal(LocalVelocity2D)), -0.6f, 0.6f, true, true);
}


