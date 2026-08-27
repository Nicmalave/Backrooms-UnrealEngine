#include "AI/BackroomsAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"

ABackroomsAIController::ABackroomsAIController()
{
    PrimaryActorTick.bCanEverTick = false;

    Perception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));

    SightConfig->SightRadius = 3000.f;
    SightConfig->LoseSightRadius = 3500.f;
    SightConfig->PeripheralVisionAngleDegrees = 90.f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    HearingConfig->HearingRange = 2500.f;
    HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
    HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;
    HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;

    Perception->ConfigureSense(*SightConfig);
    Perception->ConfigureSense(*HearingConfig);
    Perception->SetDominantSense(SightConfig->GetSenseImplementation());
}

void ABackroomsAIController::BeginPlay()
{
    Super::BeginPlay();
    if (Perception)
    {
        Perception->OnTargetPerceptionUpdated.AddDynamic(this, &ABackroomsAIController::OnPerceptionUpdated);
    }
}

void ABackroomsAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    // If we perceived the player, set as target
    if (!Actor) return;

    APawn* ControlledPawn = GetPawn();
    if (!ControlledPawn) return;

    if (Stimulus.WasSuccessfullySensed())
    {
        // Move towards actor
        MoveToActor(Actor, 100.0f);
        CurrentTarget = Actor;
    }
    else
    {
        // Lost sight; clear target and wander
        CurrentTarget = nullptr;
        StopMovement();
    }
}
