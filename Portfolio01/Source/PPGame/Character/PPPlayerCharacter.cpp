// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

APPPlayerCharacter::APPPlayerCharacter()
{
	// 스프링암 설정
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("MainSpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
	SpringArm->TargetArmLength = 140.0f;
	SpringArm->SocketOffset.Y = 55.0f;
	SpringArm->SocketOffset.Z = 65.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 10.0f;

	// 카메라 설정
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("MainCamera"));
	Camera->SetupAttachment(SpringArm);
}

void APPPlayerCharacter::BeginPlay()
{
}

void APPPlayerCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
}

void APPPlayerCharacter::Reset()
{
}

