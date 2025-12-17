// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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

	UFUNCTION(BlueprintPure, Category = "Movement")
	UCharacterMovementComponent* GetMovementComponent() const;

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

};
