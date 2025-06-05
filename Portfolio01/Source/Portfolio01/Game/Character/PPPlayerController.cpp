// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/Character/PPPlayerController.h"

APPPlayerController::APPPlayerController()
{
	bShowMouseCursor = false; // 마우스 커서 표시
}

void APPPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 게임 시작 시 초기화 코드
	UE_LOG(LogTemp, Log, TEXT("PlayerController BeginPlay"));

    PlayerCharacter = Cast<APPPlayerCharacter>(GetPawn());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayerCharacter is not set in PlayerController!"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("PlayerCharacter is set successfully!"));
	}
}

void APPPlayerController::SetupInputComponent()
{
    Super::SetupInputComponent();

    check(InputComponent);

    // 입력 바인딩 예시
    InputComponent->BindAxis("MoveForward", this, &APPPlayerController::HandleMoveForward);
    InputComponent->BindAxis("MoveRight", this, &APPPlayerController::HandleMoveRight);
 
}

void APPPlayerController::HandleMoveForward(float Value)
{
    if (PlayerCharacter)
    {
        PlayerCharacter->UseAbility("MoveForward");
    }
}

void APPPlayerController::HandleMoveRight(float Value)
{
    if (PlayerCharacter)
    {
        PlayerCharacter->UseAbility("MoveRight");
    }
}
