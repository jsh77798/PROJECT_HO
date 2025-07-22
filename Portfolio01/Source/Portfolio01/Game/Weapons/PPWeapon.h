// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PPWeapon.generated.h"

class USkeletalMeshComponent;
class UAudioComponent;
class USoundBase;
class UPPWeaponFire;


UCLASS()
class PORTFOLIO01_API APPWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	APPWeapon();

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void AddFakeProjectileData(int32 _NumerOfFakeProjectiles, float _ConeHalfAngleInDegrees = 0.5);

	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void TriggerFireAudio(USoundBase* Sound, AActor* Actor);

protected:
	virtual void BeginPlay() override;

	virtual void Fire(TArray<FVector> _ImpactPositions, TArray<FVector> _ImpactNormals, TArray<TEnumAsByte<EPhysicalSurface>> _ImpactSurfaceTypes);

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PP")
	TArray<FVector> ImpactPositions;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PP")
	TArray<FVector> ImpactNormals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PP")
	TArray<TEnumAsByte<EPhysicalSurface>> ImpactSurfaceTypes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PP")
	FVector MuzzlePosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PP")
	bool NeedsFakeProjectileData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PP")
	int32 NumerOfFakeProjectiles;

private:
	TObjectPtr<UPPWeaponFire> WeaponFire;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> SkeletalMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AudioComponent", Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAudioComponent> AudioComponent;
};
