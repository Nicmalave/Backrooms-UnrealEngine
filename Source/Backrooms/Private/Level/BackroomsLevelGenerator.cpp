#include "Level/BackroomsLevelGenerator.h"
#include "ProceduralMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

ABackroomsLevelGenerator::ABackroomsLevelGenerator()
{
    PrimaryActorTick.bCanEverTick = false;

    // Initialize level themes
    LevelThemes.SetNum(5);

    // Level 0
    LevelThemes[0].LevelName = "LEVEL 0";
    LevelThemes[0].WallColor = FLinearColor(0.53f, 0.46f, 0.22f);
    LevelThemes[0].FloorColor = FLinearColor(0.53f, 0.46f, 0.22f);
    LevelThemes[0].EntityType = "Wretch";
    LevelThemes[0].EntityCount = 1;
    LevelThemes[0].AmbientLightIntensity = 0.24f;

    // Level 1
    LevelThemes[1].LevelName = "LEVEL 1";
    LevelThemes[1].WallColor = FLinearColor(0.23f, 0.23f, 0.23f);
    LevelThemes[1].FloorColor = FLinearColor(0.18f, 0.18f, 0.18f);
    LevelThemes[1].EntityType = "Hound";
    LevelThemes[1].EntityCount = 2;
    LevelThemes[1].AmbientLightIntensity = 0.12f;

    // Level 2
    LevelThemes[2].LevelName = "LEVEL 2";
    LevelThemes[2].WallColor = FLinearColor(0.29f, 0.16f, 0.10f);
    LevelThemes[2].FloorColor = FLinearColor(0.22f, 0.14f, 0.06f);
    LevelThemes[2].EntityType = "Hound";
    LevelThemes[2].EntityCount = 2;
    LevelThemes[2].AmbientLightIntensity = 0.10f;

    // Level 5
    LevelThemes[3].LevelName = "LEVEL 5";
    LevelThemes[3].WallColor = FLinearColor(0.16f, 0.08f, 0.12f);
    LevelThemes[3].FloorColor = FLinearColor(0.13f, 0.06f, 0.07f);
    LevelThemes[3].EntityType = "Smiler";
    LevelThemes[3].EntityCount = 2;
    LevelThemes[3].AmbientLightIntensity = 0.07f;

    // Level 8
    LevelThemes[4].LevelName = "LEVEL 8";
    LevelThemes[4].WallColor = FLinearColor(0.06f, 0.06f, 0.08f);
    LevelThemes[4].FloorColor = FLinearColor(0.05f, 0.05f, 0.06f);
    LevelThemes[4].EntityType = "Smiler";
    LevelThemes[4].EntityCount = 3;
    LevelThemes[4].AmbientLightIntensity = 0.045f;
}

void ABackroomsLevelGenerator::BeginPlay()
{
    Super::BeginPlay();
}

void ABackroomsLevelGenerator::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABackroomsLevelGenerator::GenerateLevel(int32 LevelIndex)
{
    ClearLevel();

    const FLevelTheme& Theme = LevelThemes[FMath::Clamp(LevelIndex, 0, LevelThemes.Num() - 1)];

    // Procedural maze dimensions increase with level
    int32 Cols = 26 + LevelIndex * 3;
    int32 Rows = 20 + LevelIndex * 2;

    // Generate maze using recursive backtracking
    GenerateMaze(Cols, Rows, LevelIndex == 0 ? TEXT("open") : TEXT("corridor"));

    // Find exit location
    FVector2D ExitCell = FindFarthestFloor(Cols / 2, Rows / 2);

    // Place exit marker
    PlaceExit(ExitCell);

    // Place entities
    PlaceEntities(LevelIndex);

    // Add stains/debris
    PlaceStains();

    UE_LOG(LogTemp, Warning, TEXT("Generated %s (%dx%d)"), *Theme.LevelName, Cols, Rows);
}

void ABackroomsLevelGenerator::ClearLevel()
{
    // Destroy all entities and meshes from previous level
    for (ABackroomsEntity* Entity : LevelEntities)
    {
        if (Entity)
        {
            Entity->Destroy();
        }
    }
    LevelEntities.Empty();
}

void ABackroomsLevelGenerator::GenerateMaze(int32 Cols, int32 Rows, const FString& Style)
{
    // Initialize grid with all walls (0 = wall, 1 = floor)
    MazeGrid.SetNum(Rows);
    for (int32 Y = 0; Y < Rows; Y++)
    {
        MazeGrid[Y].SetNum(Cols);
        for (int32 X = 0; X < Cols; X++)
        {
            MazeGrid[Y][X] = 0;
        }
    }

    GridCols = Cols;
    GridRows = Rows;

    // Carve random paths
    int32 StartX = Cols / 2;
    int32 StartY = Rows / 2;
    CarvePath(StartX, StartY);

    int32 Steps = Style == "open" ? Cols * Rows * 2 : Cols * Rows;
    for (int32 I = 0; I < Steps; I++)
    {
        int32 Direction = FMath::Rand() % 4;
        if (Direction == 0) StartX++;
        else if (Direction == 1) StartX--;
        else if (Direction == 2) StartY++;
        else StartY--;

        StartX = FMath::Clamp(StartX, 1, Cols - 2);
        StartY = FMath::Clamp(StartY, 1, Rows - 2);
        CarvePath(StartX, StartY);
    }

    // Ensure border walls
    for (int32 X = 0; X < Cols; X++)
    {
        MazeGrid[0][X] = 0;
        MazeGrid[Rows - 1][X] = 0;
    }
    for (int32 Y = 0; Y < Rows; Y++)
    {
        MazeGrid[Y][0] = 0;
        MazeGrid[Y][Cols - 1] = 0;
    }
}

void ABackroomsLevelGenerator::CarvePath(int32 X, int32 Y)
{
    if (X > 0 && X < GridCols - 1 && Y > 0 && Y < GridRows - 1)
    {
        MazeGrid[Y][X] = 1;
    }
}

FVector2D ABackroomsLevelGenerator::FindFarthestFloor(int32 StartX, int32 StartY)
{
    // BFS to find farthest floor cell from start
    TArray<TArray<bool>> Visited;
    Visited.SetNum(GridRows);
    for (int32 Y = 0; Y < GridRows; Y++)
    {
        Visited[Y].SetNum(GridCols);
        for (int32 X = 0; X < GridCols; X++)
        {
            Visited[Y][X] = false;
        }
    }

    TQueue<FIntPoint> Queue;
    TMap<FIntPoint, int32> Distances;

    Queue.Enqueue(FIntPoint(StartX, StartY));
    Visited[StartY][StartX] = true;
    Distances.Add(FIntPoint(StartX, StartY), 0);

    FIntPoint BestCell(StartX, StartY);
    int32 BestDist = 0;

    FIntPoint Current;
    while (Queue.Dequeue(Current))
    {
        int32 CurrentDist = Distances[Current];
        if (CurrentDist > BestDist)
        {
            BestDist = CurrentDist;
            BestCell = Current;
        }

        // Check 4 neighbors
        FIntPoint Neighbors[] = {FIntPoint(Current.X + 1, Current.Y), FIntPoint(Current.X - 1, Current.Y),
                                 FIntPoint(Current.X, Current.Y + 1), FIntPoint(Current.X, Current.Y - 1)};

        for (const FIntPoint& Neighbor : Neighbors)
        {
            if (Neighbor.X >= 0 && Neighbor.X < GridCols && Neighbor.Y >= 0 && Neighbor.Y < GridRows &&
                !Visited[Neighbor.Y][Neighbor.X] && MazeGrid[Neighbor.Y][Neighbor.X] == 1)
            {
                Visited[Neighbor.Y][Neighbor.X] = true;
                Queue.Enqueue(Neighbor);
                Distances.Add(Neighbor, CurrentDist + 1);
            }
        }
    }

    return FVector2D(BestCell.X, BestCell.Y);
}

void ABackroomsLevelGenerator::PlaceEntities(int32 LevelIndex)
{
    // TODO: Spawn AI entities based on level theme
}

void ABackroomsLevelGenerator::PlaceStains()
{
    // TODO: Add debris/stains to floor
}

void ABackroomsLevelGenerator::PlaceExit(const FVector2D& ExitCell)
{
    // TODO: Place exit marker in world
}