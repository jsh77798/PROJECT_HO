// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Data/DefaultCharacterData.h"
#include "Data/AbilityData.h"
#include "Modular/ModularCharacter.h"

#include "PPCharacter.generated.h"

class AActor;
class AController;
class APPPlayerController;
class UAbilitySystemComponent;
class UInputComponent;
class UPPAbilitySystemComponent;
class UHealthComponent;
class UObject;
struct FFrame;
struct FGameplayTag;
struct FGameplayTagContainer;


UCLASS()
class PPGAME_API APPCharacter : public AModularCharacter
{
	GENERATED_BODY()

	/** CharacterName */
	UPROPERTY(Category = "Name", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName CharacterName = "NONE";

	/** CharacterData */
	struct FDefaultCharacterData* CharacterData;

	/** AbilityData */
	struct FAbilityData* AbilityData;

	/** CharacterData */
	//UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//UDataTable* CharacterData;

	/** AbilityData */
	//UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	//UDataTable* AbilityData;

	/** MappingContext */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputAction* MoveAction;

public:
	APPCharacter();
	virtual void PostInitializeComponents() override;

public:
	UFUNCTION(BlueprintCallable, Category = "Character")
	APPPlayerController* GetPPPlayerController() const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;

	// Use Ability
	UFUNCTION(BlueprintCallable, Category = "Ability")
	void UseAbility(FName AbilityName);

protected:
	

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

protected:
	/** Called for movement input */
	//void Move(const FInputActionValue& Value);

	/** Called for looking input */
	//void Look(const FInputActionValue& Value);
};
