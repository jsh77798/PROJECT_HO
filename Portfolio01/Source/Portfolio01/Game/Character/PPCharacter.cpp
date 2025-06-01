// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Global/Portfolio_GameInstance.h"
#include "HealthComponent.h"

APPCharacter::APPCharacter()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
}

void APPCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UPortfolio_GameInstance* Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();
	if (Inst != nullptr)
	{
		CharacterData = Inst->GetCharacterData(CharacterName);
	}

	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

APPPlayerController* APPCharacter::GetPPPlayerController() const
{
	return nullptr;
}

void APPCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void APPCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void APPCharacter::Reset()
{
}
