// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "GameFramework/Actor.h"
#include "HAL/Platform.h"
#include "UObject/UObjectGlobals.h"
#include "HealthComponent.generated.h"

UENUM(BlueprintType)
enum class ELyraDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS()
class PORTFOLIO01_API AHealthComponent : public AActor
{
	GENERATED_BODY()

public:
	AHealthComponent();
	
};
