#include "AI/BackroomsEntity.h"
#include "Player/BackroomsPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Math/UnrealMathUtility.h"

ABackroomsEntity::ABackroomsEntity()
{
    PrimaryActorTick.bCanEverTick = true;

    // Don't rotate with camera
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABackroomsEntity::BeginPlay()
{
    Super::BeginPlay();
}

void ABackroomsEntity::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update AI behavior based on state
    switch (CurrentState)
    {
    case EEntityState::ES_Wander:
        WanderAround();
        break;
    case EEntityState::ES_Chase:
        if (TargetPlayer)
        {
            ChasePlayer(TargetPlayer);
        }
        break;
    default:
        break;
    }
}

void ABackroomsEntity::ChasePlayer(ABackroomsPlayerCharacter* Player)
{
    if (!Player) return;

    TargetPlayer = Player;
    CurrentState = EEntityState::ES_Chase;
    GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;

    FVector PlayerLocation = Player->GetActorLocation();
    FVector DirectionToPlayer = (PlayerLocation - GetActorLocation()).GetSafeNormal();
    AddMovementInput(DirectionToPlayer, 1.0f);
}

void ABackroomsEntity::WanderAround()
{
    CurrentState = EEntityState::ES_Wander;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

    // Simple random walk behavior
    if ((WanderTarget - GetActorLocation()).Length() < 100.0f || WanderTarget == FVector::ZeroVector)
    {
        FVector RandomDirection = FVector(
            FMath::RandRange(-1.0f, 1.0f),
            FMath::RandRange(-1.0f, 1.0f),
            0.0f);
        RandomDirection.Normalize();
        WanderTarget = GetActorLocation() + RandomDirection * 1000.0f;
    }

    FVector DirectionToTarget = (WanderTarget - GetActorLocation()).GetSafeNormal();
    AddMovementInput(DirectionToTarget, 1.0f);
}