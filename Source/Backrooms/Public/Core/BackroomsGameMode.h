#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BackroomsGameMode.generated.h"

class ABackroomsPlayerCharacter;
class ABackroomsLevelGenerator;
class ABackroomsEntity;

UCLASS()
class BACKROOMS_API ABackroomsGameMode : public AGameModeBase
{
    GENERATED_BODY()

public:
    ABackroomsGameMode();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Level system
    UFUNCTION(BlueprintCallable, Category = "Backrooms")
    void LoadLevel(int32 LevelIndex);

    UFUNCTION(BlueprintCallable, Category = "Backrooms")
    void NextLevel();

    UFUNCTION(BlueprintCallable, Category = "Backrooms")
    void GameOver(const FString& Cause);

    UFUNCTION(BlueprintCallable, Category = "Backrooms")
    void WinGame();

    // Sanity and gameplay state
    UFUNCTION(BlueprintCallable, Category = "Backrooms")
    void ModifySanity(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Backrooms")
    float GetSanity() const { return CurrentSanity; }

    UFUNCTION(BlueprintCallable, Category = "Backrooms")
    int32 GetCurrentLevel() const { return CurrentLevelIndex; }

    // Delegate for UI updates
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSanityChanged, float, NewSanity, float, MaxSanity);
    UPROPERTY(BlueprintAssignable, Category = "Backrooms")
    FOnSanityChanged OnSanityChanged;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBatteryChanged, float, NewBattery, float, MaxBattery);
    UPROPERTY(BlueprintAssignable, Category = "Backrooms")
    FOnBatteryChanged OnBatteryChanged;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32, NewLevel, const FString&, LevelName);
    UPROPERTY(BlueprintAssignable, Category = "Backrooms")
    FOnLevelChanged OnLevelChanged;

private:
    float CurrentSanity = 100.0f;
    float MaxSanity = 100.0f;
    float SanityDrainRate = 3.2f;
    
    int32 CurrentLevelIndex = 0;
    
    UPROPERTY()
    ABackroomsLevelGenerator* LevelGenerator;
    
    UPROPERTY()
    ABackroomsPlayerCharacter* PlayerCharacter;

    bool bGameRunning = true;
    bool bGameWon = false;
};