// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PPAnimTypes.generated.h"

UENUM(BlueprintType)
enum class AnimEnum_CardinalDirection : uint8
{
    Forward,
    Backward,
	Left,
	Right
};

UENUM(BlueprintType)
enum class AnimEnum_RootYawOffsetMode : uint8
{
    BlendOut,
    Hold,
    Accumulate
};

USTRUCT(BlueprintType)
struct FAnimStruct_CardinalDirections
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

USTRUCT(BlueprintType)
struct FAnimStruct_TurnInPlaceEntry
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace")
    float Angle = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace")
    float AngleModifier = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace")
    UAnimSequence* AnimSequence = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace")
    UAnimSequence* CrouchAnimSequence = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace")
    float DelayBeforeTrigger = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TurnInPlace")
    FVector2D AngleRange;
};

/**
 * 
 */
class PPGAME_API PPAnimTypes
{
public:
	
};
