// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GameplayEffectTypes.h"

#include "PPAnimInstance.generated.h"

class UAbilitySystemComponent;

USTRUCT(BlueprintType)
struct FPPAnimStruct_CardinalDirections
{
    GENERATED_BODY()

public:

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cardinal")
    UAnimSequence* Forward = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cardinal")
    UAnimSequence* Backward = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cardinal")
    UAnimSequence* Left = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cardinal")
    UAnimSequence* Right = nullptr;
};


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
};
