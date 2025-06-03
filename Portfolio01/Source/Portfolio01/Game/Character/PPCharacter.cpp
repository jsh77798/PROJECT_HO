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
}

APPPlayerController* APPCharacter::GetPPPlayerController() const
{
	return nullptr;
}

void APPCharacter::BeginPlay()
{
	Super::BeginPlay();

	UPortfolio_GameInstance* Inst = GetWorld()->GetGameInstance<UPortfolio_GameInstance>();
	if (Inst != nullptr) { CharacterData = Inst->GetCharacterData(CharacterName); }

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterData->bOrientRotationToMovement; //true
	GetCharacterMovement()->RotationRate = CharacterData->RotationRate; // FRotator(0.0f, 500.0f, 0.0f); 

	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = CharacterData->MaxWalkSpeed; // 350.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = CharacterData->MinAnalogWalkSpeed; // 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = CharacterData->BrakingDecelerationWalking; // 2000.f;
}

void APPCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void APPCharacter::Reset()
{
}
