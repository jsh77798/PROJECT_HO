// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "PPCharacterMovementComponent.h"
#include "Global/Portfolio_GameInstance.h"
#include "HealthComponent.h"
#include "AI/PPAIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPCharacter)

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
	UPPCharacterMovementComponent* PPMoveComp = CastChecked<UPPCharacterMovementComponent>(GetCharacterMovement());
	PPMoveComp->bOrientRotationToMovement = CharacterData->bOrientRotationToMovement;
	PPMoveComp->RotationRate = CharacterData->RotationRate;
	// instead of recompiling to adjust them
	PPMoveComp->MaxWalkSpeed = CharacterData->MaxWalkSpeed;
	PPMoveComp->MinAnalogWalkSpeed = CharacterData->MinAnalogWalkSpeed;
	PPMoveComp->BrakingDecelerationWalking = CharacterData->BrakingDecelerationWalking;
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

