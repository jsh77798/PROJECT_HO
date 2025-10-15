// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayCueInterface.h"
#include "GameplayTagAssetInterface.h"
#include "Engine/DataTable.h"
#include "Data/DefaultCharacterData.h"
#include "Data/AbilityData.h"
#include "Modular/ModularCharacter.h"

#include "PPCharacter.generated.h"

class AActor;
class AController;
class APPPlayerController;
class UPPPawnData;
class UAbilitySystemComponent;
class UPPAbilitySystemComponent;
class UInputComponent;
class UHealthComponent;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FGameplayTagContainer;


UCLASS()
class PPGAME_API APPCharacter : public AModularCharacter, public IGameplayCueInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

	/** CharacterName */
	UPROPERTY(Category = "PP|Name", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName CharacterName = "NONE";

	/** CharacterData */
	struct FDefaultCharacterData* CharacterData;

	/** AbilityData */
	struct FAbilityData* AbilityData;

	UFUNCTION(BlueprintCallable, Category = "PP|Ability")
	UPPAbilitySystemComponent* GetPPAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;

protected:
	APPCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	void InitializeGameplayTags();

	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode) override;
	void SetMovementModeTag(EMovementMode MovementMode, uint8 CustomMovementMode, bool bTagEnabled);

public:
	UFUNCTION(BlueprintCallable, Category = "PP|Controller")
	APPPlayerController* GetPPPlayerController() const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;

	// Use Ability
	UFUNCTION(BlueprintCallable, Category = "PP|Ability")
	void UseAbility(FName AbilityName);

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PP|Health", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PP|Ability", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UPPAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Category = "PP|Pawn")
	TObjectPtr<const UPPPawnData> PawnData;
};
