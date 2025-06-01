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


/**
 * FLyraReplicatedAcceleration: Compressed representation of acceleration
 */
USTRUCT()
struct FPPReplicatedAcceleration
{
	GENERATED_BODY()

	UPROPERTY()
	uint8 AccelXYRadians = 0;	// Direction of XY accel component, quantized to represent [0, 2*pi]

	UPROPERTY()
	uint8 AccelXYMagnitude = 0;	//Accel rate of XY component, quantized to represent [0, MaxAcceleration]

	UPROPERTY()
	int8 AccelZ = 0;	// Raw Z accel rate component, quantized to represent [-MaxAcceleration, MaxAcceleration]
};


UCLASS()
class PORTFOLIO01_API APPCharacter : public AModularCharacter
{
	GENERATED_BODY()

	/** CharacterName */
	UPROPERTY(Category = "Name", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName CharacterName = "NONE";

	/** CharacterData */
	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FDefaultCharacterData CharacterData;

	/** AbilityData */
	UPROPERTY(Category = "Data", EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FAbilityData AbilityData;

	/** MappingContext */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputMappingContext* DefaultMappingContext;

	/** Move Input Action */
	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	//class UInputAction* MoveAction;

public:
	APPCharacter();

public:
	UFUNCTION(BlueprintCallable, Category = "Character")
	APPPlayerController* GetPPPlayerController() const;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	virtual void Reset() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHealthComponent> HealthComponent;

protected:
	/** Called for movement input */
	//void Move(const FInputActionValue& Value);

	/** Called for looking input */
	//void Look(const FInputActionValue& Value);
};
