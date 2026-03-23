// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "KismetAnimationLibrary.h"
#include "GameplayEffectTypes.h"
#include "PPAnimTypes.h"

#include "PPAnimInstance.generated.h"

class UAbilitySystemComponent;
class UCharacterMovementComponent;

/**
 * UPPAnimInstance
 *
 *	The base game animation instance class used by this project.
 */
UCLASS(Config = Game)
class UPPAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPPAnimInstance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeWithAbilitySystem(UAbilitySystemComponent* ASC);

protected:
#if WITH_EDITOR
	virtual EDataValidationResult IsDataValid(TArray<FText>& ValidationErrors) override;
#endif // WITH_EDITOR

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	// Gameplay tags that can be mapped to blueprint variables. The variables will automatically update as the tags are added or removed.
	// These should be used instead of manually querying for the gameplay tags.
	UPROPERTY(EditDefaultsOnly, Category = "GameplayTags")
	FGameplayTagBlueprintPropertyMap GameplayTagPropertyMap;

	UPROPERTY(BlueprintReadOnly, Category = "Character State Data")
	float GroundDistance = -1.0f;

	UPROPERTY(Transient)
	TWeakObjectPtr<APawn> CachedPawn;

	UPROPERTY(Transient)
	TWeakObjectPtr<ACharacter> CachedCharacter;

	UFUNCTION(BlueprintPure, Category = "Default")
	bool ShouldEnableControlRig();

	UFUNCTION(BlueprintPure, Category = "Default")
	UCharacterMovementComponent* GetMovementComponent();

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "HelperFunctions")
	AnimEnum_CardinalDirection SelectCardinalDirectionfromAngle(float Angle, float DeadZone, AnimEnum_CardinalDirection CurrentDirection, bool UseCurrentDirection);

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "HelperFunctions")
	AnimEnum_CardinalDirection GetOppositeCardinalDirection(AnimEnum_CardinalDirection CurrentDirection) const;

	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "HelperFunctions")
	bool IsMovingPerpendicularToInitialPivot();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateLocationData(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateRotationData();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateVelocityData();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateAccelerationData();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateCharacterStateData(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateBlendWeightData(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateAimingData();

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateWallDetectionHeuristic();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool IsFirstUpdate = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool EnableControlRig = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool UseFootPlacement = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	bool bEnableRootYawOffset = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	FRotator WorldRotation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	float YawDeltaSinceLastUpdate = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	float AdditiveLeanAngle = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "RotationData")
	float YawDeltaSpeed = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocationData")
	FVector WorldLocation;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocationData")
	float DisplacementSinceLastUpdate = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocationData")
	float DisplacementSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	FVector WorldVelocity;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	FVector LocalVelocity2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	float LocalVelocityDirectionAngle = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	float LocalVelocityDirectionAngleWithOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	AnimEnum_CardinalDirection LocalVelocityDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	AnimEnum_CardinalDirection LocalVelocityDirectionNoOffset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "VelocityData")
	bool HasVelocity = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AccelerationData")
	FVector LocalAcceleration2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AccelerationData")
	bool HasAcceleration = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AccelerationData")
	FVector PivotDirection2D;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool IsOnGround = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool IsCrouching = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool CrouchStateChange = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool ADSStateChanged = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool WasADSLastUpdate = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	float TimeSinceFiredWeapon = 9999.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "CharacterStateData")
	bool IsRunningIntoWall = false;

	FGameplayTag ADS_Tag = FGameplayTag::RequestGameplayTag(FName("Event.Movement.ADS"));

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTagBindings")
	//bool GameplayTag_IsADS = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool GameplayTag_IsFiring = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool GameplayTag_IsReloading = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "GameplayTagBindings")
	bool GameplayTag_IsMelee = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	AnimEnum_CardinalDirection StartDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	AnimEnum_CardinalDirection PivotInitialDirection;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	float LastPivotTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LocomotionSMData")
	AnimEnum_CardinalDirection CardinalDirectionFromAcceleration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlendWeightData")
	float UpperbodyDynamicAdditiveWeight = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AimingData")
	float AimPitch = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AimingData")
	float AimYaw = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LinkedLayerData")
	bool LinkedLayerChanged = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "LinkedLayerData")
	UAnimInstance* LastLinkedLayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	float RootYawOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	FFloatSpringState RootYawOffsetSpringState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	float TurnYawCurveValue = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	AnimEnum_RootYawOffsetMode RootYawOffsetMode;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	FVector2D RootYawOffsetAngleClamp = FVector2D(-120.0f, 100.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	FVector2D RootYawOffsetAngleClampCrouched = FVector2D(-90.0f, 80.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	float CardinalDirectionDeadZone = 10.0f;
};
