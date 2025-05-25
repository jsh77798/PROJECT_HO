// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/EngineTypes.h"
#include "GameFramework/Character.h"
#include "UObject/UObjectGlobals.h"

#include "ModularCharacter.generated.h"

class UObject;

UCLASS(Blueprintable)
class PORTFOLIO01_API AModularCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	//~ Begin AActor Interface
	virtual void PreInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	//~ End AActor Interface
};
