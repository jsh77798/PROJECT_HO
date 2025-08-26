// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/PPCharacter.h"

#include "PPPlayerCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;


UCLASS()
class PPGAME_API APPPlayerCharacter : public APPCharacter
{
	GENERATED_BODY()
	
	/** PlayerCharacterData */
	struct FPlayerData* PlayerData;

public:
	APPPlayerCharacter();

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "PlayerCharacter", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr <USpringArmComponent> SpringArm;
	
};
