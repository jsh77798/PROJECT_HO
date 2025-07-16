// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PPWeapon.generated.h"

class USkeletalMeshComponent;
class UAudioComponent;
class USoundBase;
//class USceneComponent;


UCLASS()
class PORTFOLIO01_API APPWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APPWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddFakeProjectileData(int32 NumerOfFakeProjectiles, float ConeHalfAngleInDegrees = 0.5);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TriggerFireAudio(USoundBase* Sound, AActor* Actor);

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	FVector ImpactPositions;

	FVector ImpactNormals;

	EPhysicalSurface ImpactSurfaceTypes;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AudioComponent", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> AudioComponent;
};
