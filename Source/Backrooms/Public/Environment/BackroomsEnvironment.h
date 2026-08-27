#pragma once

#include "CoreMinimal.h"
#include "Engine/Actor.h"
#include "BackroomsEnvironment.generated.h"

class UProceduralMeshComponent;
class UMaterialInstanceDynamic;

UCLASS()
class BACKROOMS_API ABackroomsEnvironment : public AActor
{
    GENERATED_BODY()

public:
    ABackroomsEnvironment();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Procedural mesh generation
    UFUNCTION(BlueprintCallable, Category = "Environment")
    void GenerateWalls(const TArray<TArray<uint8>>& MazeGrid, int32 GridCols, int32 GridRows, float TileSize);

    UFUNCTION(BlueprintCallable, Category = "Environment")
    void GenerateFloor(const TArray<TArray<uint8>>& MazeGrid, int32 GridCols, int32 GridRows, float TileSize);

    UFUNCTION(BlueprintCallable, Category = "Environment")
    void GenerateCeiling(int32 GridCols, int32 GridRows, float TileSize);

    UFUNCTION(BlueprintCallable, Category = "Environment")
    void ApplyMaterials(const FLinearColor& WallColor, const FLinearColor& FloorColor, const FString& LevelName);

    UFUNCTION(BlueprintCallable, Category = "Environment")
    void AddWallpaper(const FString& LevelStyle);

    UFUNCTION(BlueprintCallable, Category = "Environment")
    void AddFluorescentLighting(int32 GridCols, int32 GridRows, float TileSize);

private:
    UPROPERTY()
    UProceduralMeshComponent* WallMesh;

    UPROPERTY()
    UProceduralMeshComponent* FloorMesh;

    UPROPERTY()
    UProceduralMeshComponent* CeilingMesh;

    UPROPERTY()
    TArray<class ASpotLight*> FluorescentLights;

    UPROPERTY()
    UMaterialInstanceDynamic* WallMaterial;

    UPROPERTY()
    UMaterialInstanceDynamic* FloorMaterial;

    const float WALL_HEIGHT = 300.0f;
    const float CEILING_HEIGHT = 350.0f;

    // Mesh generation helpers
    void GenerateQuad(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals,
        TArray<FVector2D>& UVs, FVector P1, FVector P2, FVector P3, FVector P4);
};