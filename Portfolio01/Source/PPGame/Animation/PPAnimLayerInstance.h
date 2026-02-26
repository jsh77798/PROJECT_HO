// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PPAnimTypes.h"

#include "PPAnimLayerInstance.generated.h"

class UPPAnimInstance;
class UCharacterMovementComponent;

/**
 * UPPAnimLayerInstance
 *
 */
UCLASS(Config = Game)
class PPGAME_API UPPAnimLayerInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe), Category = "Default")
	UPPAnimInstance* GetMainAnimThreadSafe() const;

	UFUNCTION(BlueprintPure, Category = "Default")
	bool ShouldEnableFootPlacement() const;

	UFUNCTION(BlueprintPure, Category = "Default")
	UCharacterMovementComponent* GetMovementComponent() const;

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateBlendWeightData(float DeltaTime);

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe), Category = "BlueprintThreadSafeUpdateFunctions")
	void UpdateSkelControlData();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Idle")
	UAnimSequence* Idle = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Idle")
	UAnimSequence* Idle_ADS = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Idle")
	UAnimSequence* Idle_Hipfire = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Idle")
	TArray<UAnimSequence*> Idle_Breaks;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Idle")
	UAnimSequence* Crouch_Idle = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Idle")
	UAnimSequence* Crouch_Idle_Entry = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Idle")
	UAnimSequence* Crouch_Idle_Exit = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Starts")
	FAnimStruct_CardinalDirections Jog_Start_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Starts")
	FAnimStruct_CardinalDirections ADS_Start_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Starts")
	FAnimStruct_CardinalDirections Crouch_Start_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Stops")
	FAnimStruct_CardinalDirections Jog_Stop_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Stops")
	FAnimStruct_CardinalDirections ADS_Stop_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Stops")
	FAnimStruct_CardinalDirections Crouch_Stop_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Pivots")
	FAnimStruct_CardinalDirections Jog_Pivot_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Pivots")
	FAnimStruct_CardinalDirections ADS_Pivot_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Pivots")
	FAnimStruct_CardinalDirections Crouch_Pivot_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* TurnInPlace_Left_90 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* TurnInPlace_Right_90 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* TurnInPlace_Left_180 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* TurnInPlace_Right_180 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* Crouch_TurnInPlace_Left_90 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* Crouch_TurnInPlace_Right_90 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* Crouch_TurnInPlace_Left_180 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-TurnInPlace")
	UAnimSequence* Crouch_TurnInPlace_Right_180 = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Walk")
	FAnimStruct_CardinalDirections Walk_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Walk")
	FAnimStruct_CardinalDirections Crouch_Walk_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Jog")
	FAnimStruct_CardinalDirections Jog_Cardinals;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Aiming")
	UAnimSequence* Aim_HipFirePose = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Aiming")
	UAnimSequence* Aim_HipFirePose_Crouch = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Aiming")
	UBlendSpace* IdleAimOffset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AnimSet-Aiming")
	UBlendSpace* RelaxedAimOffset = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlendWeightData")
	float HipFireUpperBodyOverrideWeight = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BlendWeightData")
	float AimOffsetBlendWeight = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	float TurnInPlaceAnimTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	float TurnInPlaceRotationDirection = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "TurnInPlace")
	float TurnInPlaceRecoveryDirection = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IdleBreaks")
	bool WantsIdleBreak = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IdleBreaks")
	float TimeUntilNextIdleBreak = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IdleBreaks")
	int32 CurrentIdleBreakIndex = 0;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IdleBreaks")
	float IdleBreakDelayTime = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Pivots")
	FVector PivotStartingAcceleration;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "IdleBreaks")
	float TimeAtPivotStop = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SkelControlData")
	float HandIK_Right_Alpha = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SkelControlData")
	float HandIK_Left_Alpha = 1.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StrideWarping")
	float StrideWarpingStartAlpha = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StrideWarping")
	float StrideWarpingPivotAlpha = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "StrideWarping")
	float StrideWarpingCycleAlpha = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	FVector2D PlayRateClampStartsPivots = FVector2D(0.6f, 5.0f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	bool RaiseWeaponAfterFiringWhenCrouched = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	bool DisableHandIK = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	bool EnableLeftHandPoseOverride = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	float RaiseWeaponAfterFiringDuration = 0.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	float StrideWarpingBlendInDurationScaled = 0.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	float StrideWarpingBlendInStartOffset = 0.15f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	FName LocomotionDistanceCurveName = "Distance";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Set")
	FVector2D PlayRateClampCycle = FVector2D(0.8f, 1.2f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	float LeftHandPoseOverrideWeight = 0.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Default")
	float HandFKWeight = 1.0f;
};
