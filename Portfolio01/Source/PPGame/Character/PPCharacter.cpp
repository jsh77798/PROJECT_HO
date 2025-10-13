// Fill out your copyright notice in the Description page of Project Settings.


#include "PPCharacter.h"

#include "AbilitySystem/PPAbilitySystemComponent.h"
#include "AbilitySystemComponent.h"
#include "PPGameplayTags.h"
#include "PPCharacterMovementComponent.h"
#include "Global/Portfolio_GameInstance.h"
#include "HealthComponent.h"
#include "AI/PPAIController.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(PPCharacter)

APPCharacter::APPCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UPPCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));

	AIControllerClass = APPAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

UPPAbilitySystemComponent* APPCharacter::GetPPAbilitySystemComponent() const
{
	return Cast<UPPAbilitySystemComponent>(GetAbilitySystemComponent());
}

UAbilitySystemComponent* APPCharacter::GetAbilitySystemComponent() const
{
	return Cast<UPPAbilitySystemComponent>(GetAbilitySystemComponent());
	//return PawnExtComponent->GetPPAbilitySystemComponent();
}

void APPCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	if (const UPPAbilitySystemComponent* PPASC = GetPPAbilitySystemComponent())
	{
		PPASC->GetOwnedGameplayTags(TagContainer);
	}
}

bool APPCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	if (const UPPAbilitySystemComponent* PPASC = GetPPAbilitySystemComponent())
	{
		return PPASC->HasMatchingGameplayTag(TagToCheck);
	}

	return false;
}

bool APPCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UPPAbilitySystemComponent* PPASC = GetPPAbilitySystemComponent())
	{
		return PPASC->HasAllMatchingGameplayTags(TagContainer);
	}

	return false;
}

bool APPCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	if (const UPPAbilitySystemComponent* PPASC = GetPPAbilitySystemComponent())
	{
		return PPASC->HasAnyMatchingGameplayTags(TagContainer);
	}

	return false;
}

void APPCharacter::InitializeGameplayTags()
{
	// Clear tags that may be lingering on the ability system from the previous pawn.
	if (UPPAbilitySystemComponent* PPASC = GetPPAbilitySystemComponent())
	{
		const FPPGameplayTags& GameplayTags = FPPGameplayTags::Get();

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.MovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				PPASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		for (const TPair<uint8, FGameplayTag>& TagMapping : GameplayTags.CustomMovementModeTagMap)
		{
			if (TagMapping.Value.IsValid())
			{
				PPASC->SetLooseGameplayTagCount(TagMapping.Value, 0);
			}
		}

		UPPCharacterMovementComponent* PPMoveComp = CastChecked<UPPCharacterMovementComponent>(GetCharacterMovement());
		SetMovementModeTag(PPMoveComp->MovementMode, PPMoveComp->CustomMovementMode, true);
	}
}

void APPCharacter::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);

	UPPCharacterMovementComponent* PPMoveComp = CastChecked<UPPCharacterMovementComponent>(GetCharacterMovement());

	SetMovementModeTag(PrevMovementMode, PreviousCustomMode, false);
	SetMovementModeTag(PPMoveComp->MovementMode, PPMoveComp->CustomMovementMode, true);
}

void APPCharacter::SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled)
{
	if (UPPAbilitySystemComponent* PPASC = GetPPAbilitySystemComponent())
	{
		const FPPGameplayTags& GameplayTags = FPPGameplayTags::Get();
		const FGameplayTag* MovementModeTag = nullptr;

		if (MovementMode == MOVE_Custom)
		{
			MovementModeTag = GameplayTags.CustomMovementModeTagMap.Find(CustomMovementMode);
		}
		else
		{
			MovementModeTag = GameplayTags.MovementModeTagMap.Find(MovementMode);
		}

		if (MovementModeTag && MovementModeTag->IsValid())
		{
			PPASC->SetLooseGameplayTagCount(*MovementModeTag, (bTagEnabled ? 1 : 0));
		}
	}
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

