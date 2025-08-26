// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Character/PPPlayerCharacter.h"

#include "PPPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PPGAME_API APPPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
    APPPlayerController();

    APPPlayerCharacter* PlayerCharacter;

protected:
    virtual void BeginPlay() override;
    virtual void SetupInputComponent() override;

    // 예: 키 입력 처리 함수
    void HandleMoveForward(float Value);
    void HandleMoveRight(float Value);
};
