#include "AI/BackroomsEntity.h"
#include "Player/BackroomsPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SkeletalMeshActor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Math/UnrealMathUtility.h"

ABackroomsEntity::ABackroomsEntity()
{
    PrimaryActorTick.bCanEverTick = true;

    // Use movement component
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    
    // Skeletal mesh for animations
    SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
    RootComponent = SkeletalMesh;
}

void ABackroomsEntity::BeginPlay()
{
    Super::BeginPlay();

    // Set initial stats based on entity type
    switch (EntityType)
    {
    case EEntityType::ET_Wretch:
        EntityStats.WalkSpeed = 550.0f;
        EntityStats.ChaseSpeed = 1500.0f;
        EntityStats.SenseRadius = 1700.0f;
        EntityStats.VisibilityThreshold = 1.1f;
        break;
    case EEntityType::ET_Hound:
        EntityStats.WalkSpeed = 700.0f;
        EntityStats.ChaseSpeed = 1950.0f;
        EntityStats.SenseRadius = 2300.0f;
        EntityStats.VisibilityThreshold = 0.0f; // Hunts by sound
        break;
    case EEntityType::ET_Smiler:
        EntityStats.WalkSpeed = 400.0f;
        EntityStats.ChaseSpeed = 2300.0f;
        EntityStats.SenseRadius = 1400.0f;
        EntityStats.VisibilityThreshold = 1.6f; // Triggered by flashlight
        break;
    }
}

void ABackroomsEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update entity-specific behavior
    switch (EntityType)
    {
    case EEntityType::ET_Wretch:
        UpdateWretchBehavior(DeltaTime);
        break;
    case EEntityType::ET_Hound:
        UpdateHoundBehavior(DeltaTime);
        break;
    case EEntityType::ET_Smiler:
        UpdateSmilerBehavior(DeltaTime);
        break;
    }

    TeleportCooldown = FMath::Max(0.0f, TeleportCooldown - DeltaTime);
}

void ABackroomsEntity::UpdateWretchBehavior(float DeltaTime)
{
    // The Wretch: Tall, faceless, hunts with vision
    if (CurrentState == EEntityState::ES_Wander)
    {
        WanderAround();
    }
    else if (CurrentState == EEntityState::ES_Chase && TargetPlayer)
    {
        ChasePlayer(TargetPlayer);
        
        // Check if player escaped
        float DistToPlayer = FVector::Dist(GetActorLocation(), TargetPlayer->GetActorLocation());
        if (DistToPlayer > EntityStats.SenseRadius * 1.8f)
        {
            AlertTimer += DeltaTime;
            if (AlertTimer > 3.5f)
            {
                CurrentState = EEntityState::ES_Wander;
                AlertTimer = 0.0f;
                TargetPlayer = nullptr;
            }
        }
        else
        {
            AlertTimer = 0.0f;
        }
    }
}

void ABackroomsEntity::UpdateHoundBehavior(float DeltaTime)
{
    // The Hound: Fast, hunts by hearing/movement
    if (CurrentState == EEntityState::ES_Wander)
    {
        WanderAround();
    }
    else if (CurrentState == EEntityState::ES_Chase && TargetPlayer)
    {
        ChasePlayer(TargetPlayer);
        
        // Lost player?
        if (!CanSeePlayer(TargetPlayer))
        {
            CurrentState = EEntityState::ES_Wander;
            TargetPlayer = nullptr;
        }
    }
}

void ABackroomsEntity::UpdateSmilerBehavior(float DeltaTime)
{
    // The Smiler: Triggered by flashlight, can teleport
    if (CurrentState == EEntityState::ES_Chase && TargetPlayer)
    {
        ChasePlayer(TargetPlayer);

        // Try to teleport if can't see player
        if (!CanSeePlayer(TargetPlayer) && TeleportCooldown <= 0.0f)
        {
            TeleportCooldown = 5.0f + FMath::FRand() * 4.0f;
            
            // Teleport near player
            FVector TeleportDir = FMath::VRand();
            TeleportDir.Z = 0.0f;
            FVector NewLocation = TargetPlayer->GetActorLocation() + TeleportDir * FMath::RandRange(1500.0f, 2200.0f);
            SetActorLocation(NewLocation);
        }
    }
    else if (CurrentState == EEntityState::ES_Wander)
    {
        WanderAround();
    }
}

void ABackroomsEntity::DetectPlayer(ABackroomsPlayerCharacter* Player)
{
    if (!Player) return;

    float DistToPlayer = FVector::Dist(GetActorLocation(), Player->GetActorLocation());
    bool bInSenseRadius = DistToPlayer < EntityStats.SenseRadius;

    if (!bInSenseRadius)
    {
        CurrentState = EEntityState::ES_Wander;
        TargetPlayer = nullptr;
        return;
    }

    if (CanSeePlayer(Player))
    {
        CurrentState = EEntityState::ES_Chase;
        TargetPlayer = Player;
    }
}

bool ABackroomsEntity::CanSeePlayer(ABackroomsPlayerCharacter* Player) const
{
    if (!Player) return false;

    FVector PlayerLoc = Player->GetActorLocation();
    FVector EntityLoc = GetActorLocation();
    
    // Line trace for line-of-sight check
    FHitResult HitResult;
    FVector Start = EntityLoc;
    FVector End = PlayerLoc;
    
    FCollisionQueryParams QueryParams;
    QueryParams.AddIgnoredActor(const_cast<ABackroomsEntity*>(this));
    QueryParams.AddIgnoredActor(Player);

    bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);
    
    return !bHit || HitResult.GetActor() == Player;
}

void ABackroomsEntity::ChasePlayer(ABackroomsPlayerCharacter* Player)
{
    if (!Player) return;

    CurrentState = EEntityState::ES_Chase;
    FVector DirectionToPlayer = (Player->GetActorLocation() - GetActorLocation()).GetSafeNormal();
    
    // Move toward player at chase speed
    FVector NewLocation = GetActorLocation() + DirectionToPlayer * EntityStats.ChaseSpeed * GetWorld()->DeltaTimeSeconds;
    SetActorLocation(NewLocation);
}

void ABackroomsEntity::WanderAround()
{
    CurrentState = EEntityState::ES_Wander;

    if ((WanderTarget - GetActorLocation()).Length() < 100.0f || WanderTarget == FVector::ZeroVector)
    {
        FVector RandomDir = FMath::VRand();
        RandomDir.Z = 0.0f;
        RandomDir.Normalize();
        WanderTarget = GetActorLocation() + RandomDir * FMath::RandRange(1000.0f, 2200.0f);
    }

    FVector DirectionToTarget = (WanderTarget - GetActorLocation()).GetSafeNormal();
    FVector NewLocation = GetActorLocation() + DirectionToTarget * EntityStats.WalkSpeed * GetWorld()->DeltaTimeSeconds;
    SetActorLocation(NewLocation);
}