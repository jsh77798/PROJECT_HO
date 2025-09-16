// Fill out your copyright notice in the Description page of Project Settings.
//참조사이트 https://dlaiml.tistory.com/entry/UE5-AI-Perception-C
//           https://unreal6.tistory.com/m/209


#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "PPAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;
class UAISenseConfig_Damage;


UENUM(BlueprintType)
enum class EAIPerceptionSense : uint8
{
	EPS_NONE UMETA(DisplayName = "NONE"),
	EPS_SIGHT UMETA(DisplayName = "EPS_SIGHT"),
	EPS_HEARING UMETA(DisplayName = "EPS_HEARING"),
	EPS_DAMAGE UMETA(DisplayName = "EPS_DAMAGE"),
	EPS_MAX UMETA(DisplayName = "EPS_MAX"),
};


/**
 * 
 */
UCLASS()
class PPGAME_API APPAIController : public AAIController
{
	GENERATED_BODY()

public:
	APPAIController();

	// 빙의 시점
	virtual void OnPossess(APawn* InPawn) override; 
	// 빙의 해제
	virtual void OnUnPossess() override;

	static const FName OriginPosKey;
	static const FName PatrolPosKey;
	static const FName TargetKey;

public:
	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	UFUNCTION()
	FAIStimulus CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense);

	void HandleSensedSight(AActor* Actor);
	void HandleSensedHearing(FVector NoiseLocation);
	void HandleSensedDamage(AActor* Actor);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	TObjectPtr<UAISenseConfig_Sight> SightConfig;
	TObjectPtr<UAISenseConfig_Hearing> HearingConfig;
	TObjectPtr<UAISenseConfig_Damage> DamageSenseConfig;

private:
	UPROPERTY()
	class UBlackboardData* BBAsset;

	UPROPERTY()
	class UBehaviorTree* BTAsset;
};
