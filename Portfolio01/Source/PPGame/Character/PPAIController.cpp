// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PPAIController.h"

#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Damage.h"
#include "BehaviorTree/BehaviorTreeComponent.h"


APPAIController::APPAIController()
{
    AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*AIPerception);

    // Sight Config
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 800.f;
    SightConfig->LoseSightRadius = 1200.f;
    SightConfig->PeripheralVisionAngleDegrees = 60.f;
    SightConfig->SetMaxAge(5.f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.f;

    // Detect only specific actors 
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    AIPerception->ConfigureSense(*SightConfig);
    AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
    HearingConfig->HearingRange = 500.f;
    HearingConfig->SetMaxAge(3.f);

    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
    AIPerception->ConfigureSense(*HearingConfig);
    // ...

    AIPerception->OnPerceptionUpdated.AddDynamic(this, &APPAIController::PerceptionUpdated);

    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void APPAIController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    for (AActor* UpdatedActor : UpdatedActors)
    {
        FAIStimulus AIStimulus;
        AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_SIGHT);
        if (AIStimulus.WasSuccessfullySensed())
        {
            HandleSensedSight(UpdatedActor);
        }
        AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_HEARING);
        if (AIStimulus.WasSuccessfullySensed())
        {
            HandleSensedHearing(AIStimulus.StimulusLocation);
        }
        AIStimulus = CanSenseActor(UpdatedActor, EAIPerceptionSense::EPS_DAMAGE);
        if (AIStimulus.WasSuccessfullySensed())
        {
            HandleSensedDamage(UpdatedActor);
        }
    }
}

FAIStimulus APPAIController::CanSenseActor(AActor* Actor, EAIPerceptionSense AIPerceptionSense)
{
    FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
    FAIStimulus ResultStimulus;

    AIPerception->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);

    TSubclassOf<UAISense> QuerySenseClass;
    switch (AIPerceptionSense)
    {
    case EAIPerceptionSense::EPS_NONE:
        break;
    case EAIPerceptionSense::EPS_SIGHT:
        QuerySenseClass = UAISense_Sight::StaticClass();
        break;
    case EAIPerceptionSense::EPS_HEARING:
        QuerySenseClass = UAISense_Hearing::StaticClass();
        break;
    case EAIPerceptionSense::EPS_DAMAGE:
        QuerySenseClass = UAISense_Damage::StaticClass();
        break;
    case EAIPerceptionSense::EPS_MAX:
        break;
    default:
        break;
    }

    TSubclassOf<UAISense> LastSensedStimulusClass;

    for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
    {
        LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);


        if (QuerySenseClass == LastSensedStimulusClass)
        {
            ResultStimulus = AIStimulus;
            return ResultStimulus;
        }

    }
    return ResultStimulus;
}

void APPAIController::HandleSensedSight(AActor* Actor)
{
    // ...
    //if (bConvertToAttack && PlayerCharacter != nullptr && PlayerCharacter == Actor)
    //{
    //    SwitchToAttackState(Actor);
    //}
    // ...
}

void APPAIController::HandleSensedHearing(FVector NoiseLocation)
{
    // ...
    //SwitchToInvestigateState(NoiseLocation);
    // ...
}

void APPAIController::HandleSensedDamage(AActor* Actor)
{
	// ...
	//SwitchToAttackState(Actor);
	// ...
}
// ...