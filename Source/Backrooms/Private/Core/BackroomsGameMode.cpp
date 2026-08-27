#include "Core/BackroomsGameMode.h"
#include "Player/BackroomsPlayerCharacter.h"
#include "Level/BackroomsLevelGenerator.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Engine/World.h"

ABackroomsGameMode::ABackroomsGameMode()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickInterval = 0.016f; // ~60 FPS
}

void ABackroomsGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Spawn level generator
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    LevelGenerator = GetWorld()->SpawnActor<ABackroomsLevelGenerator>(SpawnParams);

    // Load first level
    LoadLevel(0);
}

void ABackroomsGameMode::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bGameRunning) return;

    // Apply sanity drain
    float DrainThisFrame = SanityDrainRate * DeltaTime;
    
    if (PlayerCharacter && PlayerCharacter->IsFlashlightOn())
    {
        DrainThisFrame *= 0.35f; // Reduced drain with flashlight
    }

    ModifySanity(-DrainThisFrame);

    // Check sanity threshold for game over
    if (CurrentSanity <= 0.0f)
    {
        GameOver(TEXT("madness"));
    }
}

void ABackroomsGameMode::LoadLevel(int32 LevelIndex)
{
    CurrentLevelIndex = LevelIndex;
    CurrentSanity = FMath::Min(CurrentSanity + 18.0f, MaxSanity);

    if (LevelGenerator)
    {
        LevelGenerator->GenerateLevel(LevelIndex);
    }

    OnLevelChanged.Broadcast(LevelIndex, FString::Printf(TEXT("LEVEL %d"), LevelIndex));
}

void ABackroomsGameMode::NextLevel()
{
    if (CurrentLevelIndex + 1 >= 5) // Total of 5 levels
    {
        WinGame();
        return;
    }

    LoadLevel(CurrentLevelIndex + 1);
}

void ABackroomsGameMode::ModifySanity(float Amount)
{
    CurrentSanity = FMath::Clamp(CurrentSanity + Amount, 0.0f, MaxSanity);
    OnSanityChanged.Broadcast(CurrentSanity, MaxSanity);
}

void ABackroomsGameMode::GameOver(const FString& Cause)
{
    bGameRunning = false;
    
    if (Cause == TEXT("madness"))
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over: Lost to madness"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Game Over: Caught by %s"), *Cause);
    }

    // Trigger end screen (handled by UI)
}

void ABackroomsGameMode::WinGame()
{
    bGameRunning = false;
    bGameWon = true;
    UE_LOG(LogTemp, Warning, TEXT("Game Won!"));
}