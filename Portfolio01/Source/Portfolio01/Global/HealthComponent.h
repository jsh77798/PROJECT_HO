// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/GameFrameworkComponent.h"
#include "Delegates/Delegate.h"
#include "GameFramework/Actor.h"
#include "HAL/Platform.h"
#include "UObject/UObjectGlobals.h"

#include "HealthComponent.generated.h"

class UPPAbilitySystemComponent;
class UHealthSet;

UENUM(BlueprintType)
enum class EDeathState : uint8
{
	NotDead = 0,
	DeathStarted,
	DeathFinished
};

UCLASS(Blueprintable, Meta = (BlueprintSpawnableComponent))
class PORTFOLIO01_API UHealthComponent : public UGameFrameworkComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent(const FObjectInitializer& ObjectInitializer);
	
	// Returns the health component if one exists on the specified actor.
	UFUNCTION(BlueprintPure, Category = "Health")
	static UHealthComponent* FindHealthComponent(const AActor* Actor) { return (Actor ? Actor->FindComponentByClass<UHealthComponent>() : nullptr); }

	// Uninitialize the component, clearing any references to the ability system.
	UFUNCTION(BlueprintCallable, Category = "Health")
	void UninitializeFromAbilitySystem();

	// Returns the current health value.
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealth() const;

	// Returns the current maximum health value.
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetMaxHealth() const;

	// Returns the current health in the range [0.0, 1.0].
	UFUNCTION(BlueprintCallable, Category = "Health")
	float GetHealthNormalized() const;

	UFUNCTION(BlueprintCallable, Category = "Health")
	EDeathState GetDeathState() const { return DeathState; }

	UFUNCTION(BlueprintCallable, BlueprintPure = false, Category = "Lyra|Health", Meta = (ExpandBoolAsExecs = "ReturnValue"))
	bool IsDeadOrDying() const { return (DeathState > EDeathState::NotDead); }

	// Begins the death sequence for the owner.
	virtual void StartDeath();

	// Ends the death sequence for the owner.
	virtual void FinishDeath();

	// Applies enough damage to kill the owner.
	virtual void DamageSelfDestruct(bool bFellOutOfWorld = false);

public:
	/*
	// Delegate fired when the health value has changed.
	UPROPERTY(BlueprintAssignable)
	FHealth_AttributeChanged OnHealthChanged;

	// Delegate fired when the max health value has changed.
	UPROPERTY(BlueprintAssignable)
	FHealth_AttributeChanged OnMaxHealthChanged;

	// Delegate fired when the death sequence has started.
	UPROPERTY(BlueprintAssignable)
	FHealth_DeathEvent OnDeathStarted;

	// Delegate fired when the death sequence has finished.
	UPROPERTY(BlueprintAssignable)
	FHealth_DeathEvent OnDeathFinished;
	*/

protected:

	UFUNCTION()
	virtual void OnRep_DeathState(EDeathState OldDeathState);

protected:

	// Ability system used by this component.
	UPROPERTY()
	TObjectPtr<UPPAbilitySystemComponent> AbilitySystemComponent;

	// Health set used by this component.
	UPROPERTY()
	TObjectPtr<const UHealthSet> HealthSet;

	// Replicated state used to handle dying.
	UPROPERTY()
	EDeathState DeathState;
};
