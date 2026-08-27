#include "AI/BackroomsEntity.h"
#include "AI/BackroomsAIController.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "GameFramework/CharacterMovementComponent.h"

ABackroomsEntity::ABackroomsEntity()
{
    PrimaryActorTick.bCanEverTick = true;

    // Use Character movement when possible; if this pawn isn't a character, movement calls will no-op
}

void ABackroomsEntity::BeginPlay()
{
    Super::BeginPlay();

    // Try to possess by an AIController if none
    if (!GetController())
    {
        SpawnDefaultController();
    }

    // Initialize state
    CurrentState = EEntityState::ES_Wander;
}

void ABackroomsEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Basic state machine
    switch (CurrentState)
    {
    case EEntityState::ES_Wander:
        UpdateWretchBehavior(DeltaTime);
        break;
    case EEntityState::ES_Alert:
        // slow down and look around
        AlertTimer -= DeltaTime;
        if (AlertTimer <= 0.0f)
        {
            CurrentState = EEntityState::ES_Wander;
        }
        break;
    case EEntityState::ES_Chase:
        if (TargetPlayer)
        {
            // move towards player using Nav
            AAIController* AICon = Cast<AAIController>(GetController());
            if (AICon && TargetPlayer)
            {
                AICon->MoveToActor(TargetPlayer, 80.0f);
            }
        }
        break;
    case EEntityState::ES_Hide:
        // find nearest dark corner logic can go here
        break;
    default:
        break;
    }

    // Teleport cooldown decrement
    TeleportCooldown = FMath::Max(0.0f, TeleportCooldown - DeltaTime);
}

void ABackroomsEntity::DetectPlayer(ABackroomsPlayerCharacter* Player)
{
    if (!Player) return;
    TargetPlayer = Player;
    CurrentState = EEntityState::ES_Alert;
    AlertTimer = 2.0f + FMath::FRandRange(0.0f, 2.0f);

    // If within chase radius, enter chase
    FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
    float Dist = ToPlayer.Size();
    if (Dist <= EntityStats.SenseRadius * 0.6f)
    {
        CurrentState = EEntityState::ES_Chase;
    }
}

void ABackroomsEntity::ChasePlayer(ABackroomsPlayerCharacter* Player)
{
    if (!Player) return;
    TargetPlayer = Player;
    CurrentState = EEntityState::ES_Chase;
}

void ABackroomsEntity::WanderAround()
{
    // pick random reachable point
    if (!GetWorld()) return;
    UNavigationSystemV1* Nav = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    if (!Nav) return;

    FVector Origin = GetActorLocation();
    FVector RandomPoint;
    FNavLocation Result;
    if (Nav->GetRandomReachablePointInRadius(Origin, 1500.0f, Result))
    {
        RandomPoint = Result.Location;
        AAIController* AICon = Cast<AAIController>(GetController());
        if (AICon)
        {
            AICon->MoveToLocation(RandomPoint);
        }
    }
}

bool ABackroomsEntity::CanSeePlayer(ABackroomsPlayerCharacter* Player) const
{
    if (!Player) return false;
    FVector Dir = Player->GetActorLocation() - GetActorLocation();
    float Dist = Dir.Size();
    if (Dist > EntityStats.SenseRadius) return false;

    // simple line of sight
    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);
    Params.AddIgnoredActor(Player);
    bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation() + FVector(0,0,50), Player->GetActorLocation() + FVector(0,0,50), ECC_Visibility, Params);
    return !bHit;
}

void ABackroomsEntity::UpdateWretchBehavior(float DeltaTime)
{
    // Wretch wanders slowly and occasionally emits a sound
    WanderTarget = WanderTarget + FVector(FMath::FRandRange(-1.f,1.f), FMath::FRandRange(-1.f,1.f), 0.f) * 300.0f;
    WanderAround();
}

void ABackroomsEntity::UpdateHoundBehavior(float DeltaTime)
{
    // Hound prefers to sprint towards noisy locations - placeholder
}

void ABackroomsEntity::UpdateSmilerBehavior(float DeltaTime)
{
    // Smiler teleports occasionally if player is not in LOS - placeholder
}

void ABackroomsEntity::UpdateLurkerBehavior(float DeltaTime)
{
    // Lurker hides until player is close, then ambushes
}

void ABackroomsEntity::UpdatePoolShadowBehavior(float DeltaTime)
{
    // PoolShadow drifts slowly and moves faster when in water
}
