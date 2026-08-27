#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackroomsLevelGenerator.generated.h"

class ABackroomsEntity;

USTRUCT(BlueprintType)
struct FLevelTheme
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString LevelName = "LEVEL 0";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor WallColor = FLinearColor(0.47f, 0.41f, 0.18f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor WallDarkColor = FLinearColor(0.29f, 0.25f, 0.10f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FLinearColor FloorColor = FLinearColor(0.53f, 0.46f, 0.22f);

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString EntityType = "Wretch";

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 EntityCount = 1;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float AmbientLightIntensity = 0.24f;
};

UCLASS()
class BACKROOMS_API ABackroomsLevelGenerator : public AActor
{
    GENERATED_BODY()

public:
    ABackroomsLevelGenerator();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UFUNCTION(BlueprintCallable, Category = "Level Generation")
    void GenerateLevel(int32 LevelIndex);

    UFUNCTION(BlueprintCallable, Category = "Level Generation")
    void ClearLevel();

private:
    // Maze generation
    void GenerateMaze(int32 Cols, int32 Rows, const FString& Style);
    void CarvePath(int32 X, int32 Y);
    FVector2D FindFarthestFloor(int32 StartX, int32 StartY);

    // Level decoration
    void PlaceEntities(int32 LevelIndex);
    void PlaceStains();
    void PlaceExit(const FVector2D& ExitCell);

    // Level data
    TArray<TArray<uint8>> MazeGrid;
    int32 GridCols = 0;
    int32 GridRows = 0;

    const float TILE_SIZE = 500.0f; // Unreal units

    UPROPERTY()
    TArray<ABackroomsEntity*> LevelEntities;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
    TArray<FLevelTheme> LevelThemes;
};