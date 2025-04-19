// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EventActor.generated.h"

UCLASS()
class PORTFOLIO01_API AEventActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEventActor();

	// 사용함수
	UFUNCTION()
	void Attack_LineTrace(FVector CameraLoc, FVector CameraForward, FName AttackType, FName DamageType);

	UFUNCTION()
	void OtherDamage(FHitResult HitResult_Other);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
