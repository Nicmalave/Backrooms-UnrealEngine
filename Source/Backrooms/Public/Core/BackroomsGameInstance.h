#pragma once
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "BackroomsGameInstance.generated.h"

UCLASS()
class BACKROOMS_API UBackroomsGameInstance : public UGameInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Progression")
    TArray<int32> CompletedLevels;

    UFUNCTION(BlueprintCallable, Category = "Progression")
    void MarkLevelCompleted(int32 LevelIndex)
    {
        if (!CompletedLevels.Contains(LevelIndex))
        {
            CompletedLevels.Add(LevelIndex);
        }
    }

    UFUNCTION(BlueprintCallable, Category = "Progression")
    bool IsLevelCompleted(int32 LevelIndex) const
    {
        return CompletedLevels.Contains(LevelIndex);
    }
};
