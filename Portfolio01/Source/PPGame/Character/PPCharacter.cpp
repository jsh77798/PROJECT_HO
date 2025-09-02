// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Global/Portfolio_GameInstance.h"
#include "HealthComponent.h"
#include "PPAIController.h"

APPCharacter::APPCharacter()
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	AIControllerClass = APPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
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
	if (Inst != nullptr) { AbilityData = Inst->GetAbilityData(CharacterName); }
	
	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterData->bOrientRotationToMovement; 
	GetCharacterMovement()->RotationRate = CharacterData->RotationRate; 
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = CharacterData->MaxWalkSpeed; 
	GetCharacterMovement()->MinAnalogWalkSpeed = CharacterData->MinAnalogWalkSpeed; 
	GetCharacterMovement()->BrakingDecelerationWalking = CharacterData->BrakingDecelerationWalking; 
}

void APPCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void APPCharacter::Reset()
{
}

void APPCharacter::UseAbility(FName AbilityName)
{
	if (AbilityData) { AbilityData->Ability[AbilityName]; }
}

