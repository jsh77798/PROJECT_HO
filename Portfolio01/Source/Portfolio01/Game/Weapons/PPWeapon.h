// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PPWeapon.generated.h"

class USkeletalMeshComponent;
class UAudioComponent;


UCLASS()
class PORTFOLIO01_API APPWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APPWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddFakeProjectileData();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TriggerFireAudio();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AudioComponent", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> AudioComponent;
};
