#include "Environment/BackroomsEnvironment.h"
#include "ProceduralMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Engine/SpotLight.h"
#include "Components/SpotLightComponent.h"
#include "Engine/World.h"
#include "Math/UnrealMathUtility.h"

ABackroomsEnvironment::ABackroomsEnvironment()
{
    PrimaryActorTick.bCanEverTick = false;

    // Root component
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    // Procedural mesh components
    WallMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("WallMesh"));
    WallMesh->SetupAttachment(RootComponent);

    FloorMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("FloorMesh"));
    FloorMesh->SetupAttachment(RootComponent);

    CeilingMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("CeilingMesh"));
    CeilingMesh->SetupAttachment(RootComponent);
}

void ABackroomsEnvironment::BeginPlay()
{
    Super::BeginPlay();
}

void ABackroomsEnvironment::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void ABackroomsEnvironment::GenerateWalls(const TArray<TArray<uint8>>& MazeGrid, int32 GridCols, int32 GridRows, float TileSize)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;
    TArray<FLinearColor> VertexColors;

    // Iterate through grid and generate wall quads
    for (int32 Y = 0; Y < GridRows; Y++)
    {
        for (int32 X = 0; X < GridCols; X++)
        {
            if (MazeGrid[Y][X] == 0) // Wall cell
            {
                float X0 = X * TileSize;
                float Y0 = Y * TileSize;
                float X1 = X0 + TileSize;
                float Y1 = Y0 + TileSize;

                // Front face
                FVector P1(X0, Y0, 0);
                FVector P2(X1, Y0, 0);
                FVector P3(X1, Y0, WALL_HEIGHT);
                FVector P4(X0, Y0, WALL_HEIGHT);
                GenerateQuad(Vertices, Triangles, Normals, UVs, P1, P2, P3, P4);

                // Back face
                P1 = FVector(X1, Y1, 0);
                P2 = FVector(X0, Y1, 0);
                P3 = FVector(X0, Y1, WALL_HEIGHT);
                P4 = FVector(X1, Y1, WALL_HEIGHT);
                GenerateQuad(Vertices, Triangles, Normals, UVs, P1, P2, P3, P4);

                // Left face
                P1 = FVector(X0, Y1, 0);
                P2 = FVector(X0, Y0, 0);
                P3 = FVector(X0, Y0, WALL_HEIGHT);
                P4 = FVector(X0, Y1, WALL_HEIGHT);
                GenerateQuad(Vertices, Triangles, Normals, UVs, P1, P2, P3, P4);

                // Right face
                P1 = FVector(X1, Y0, 0);
                P2 = FVector(X1, Y1, 0);
                P3 = FVector(X1, Y1, WALL_HEIGHT);
                P4 = FVector(X1, Y0, WALL_HEIGHT);
                GenerateQuad(Vertices, Triangles, Normals, UVs, P1, P2, P3, P4);
            }
        }
    }

    if (Vertices.Num() > 0)
    {
        WallMesh->CreateMeshSection_LinearColor(0, Vertices, Triangles, Normals, UVs, VertexColors, TArray<FProcMeshTangent>(), true);
        WallMesh->SetCollisionEnabled(ECC_WorldStatic);
    }
}

void ABackroomsEnvironment::GenerateFloor(const TArray<TArray<uint8>>& MazeGrid, int32 GridCols, int32 GridRows, float TileSize)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;

    // Generate floor tiles for walkable areas
    for (int32 Y = 0; Y < GridRows; Y++)
    {
        for (int32 X = 0; X < GridCols; X++)
        {
            if (MazeGrid[Y][X] == 1) // Floor cell
            {
                float X0 = X * TileSize;
                float Y0 = Y * TileSize;
                float X1 = X0 + TileSize;
                float Y1 = Y0 + TileSize;

                FVector P1(X0, Y0, 0);
                FVector P2(X1, Y0, 0);
                FVector P3(X1, Y1, 0);
                FVector P4(X0, Y1, 0);
                GenerateQuad(Vertices, Triangles, Normals, UVs, P1, P2, P3, P4);
            }
        }
    }

    if (Vertices.Num() > 0)
    {
        FloorMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
        FloorMesh->SetCollisionEnabled(ECC_WorldStatic);
    }
}

void ABackroomsEnvironment::GenerateCeiling(int32 GridCols, int32 GridRows, float TileSize)
{
    TArray<FVector> Vertices;
    TArray<int32> Triangles;
    TArray<FVector> Normals;
    TArray<FVector2D> UVs;

    // Generate ceiling
    for (int32 Y = 0; Y < GridRows; Y++)
    {
        for (int32 X = 0; X < GridCols; X++)
        {
            float X0 = X * TileSize;
            float Y0 = Y * TileSize;
            float X1 = X0 + TileSize;
            float Y1 = Y0 + TileSize;

            FVector P1(X0, Y1, CEILING_HEIGHT);
            FVector P2(X1, Y1, CEILING_HEIGHT);
            FVector P3(X1, Y0, CEILING_HEIGHT);
            FVector P4(X0, Y0, CEILING_HEIGHT);
            GenerateQuad(Vertices, Triangles, Normals, UVs, P1, P2, P3, P4);
        }
    }

    if (Vertices.Num() > 0)
    {
        CeilingMesh->CreateMeshSection(0, Vertices, Triangles, Normals, UVs, TArray<FLinearColor>(), TArray<FProcMeshTangent>(), true);
        CeilingMesh->SetCollisionEnabled(ECC_WorldStatic);
    }
}

void ABackroomsEnvironment::ApplyMaterials(const FLinearColor& WallColor, const FLinearColor& FloorColor, const FString& LevelName)
{
    // Create dynamic material instances
    WallMaterial = WallMesh->CreateDynamicMaterialInstance(0);
    if (WallMaterial)
    {
        WallMaterial->SetVectorParameterValue(FName("BaseColor"), WallColor);
        WallMaterial->SetScalarParameterValue(FName("Roughness"), 0.85f);
        WallMaterial->SetScalarParameterValue(FName("Metallic"), 0.0f);
    }

    FloorMaterial = FloorMesh->CreateDynamicMaterialInstance(0);
    if (FloorMaterial)
    {
        FLinearColor DampFloorColor = FloorColor * 0.7f; // Damper appearance
        FloorMaterial->SetVectorParameterValue(FName("BaseColor"), DampFloorColor);
        FloorMaterial->SetScalarParameterValue(FName("Roughness"), 0.95f); // Very rough, worn carpet
        FloorMaterial->SetScalarParameterValue(FName("Metallic"), 0.0f);
    }
}

void ABackroomsEnvironment::AddWallpaper(const FString& LevelStyle)
{
    if (LevelStyle == TEXT("wallpaper") && WallMaterial)
    {
        // Add subtle wallpaper pattern
        WallMaterial->SetScalarParameterValue(FName("WallpaperScale"), 0.5f);
        WallMaterial->SetScalarParameterValue(FName("WallpaperOpacity"), 0.3f);
    }
    else if (LevelStyle == TEXT("concrete") && WallMaterial)
    {
        WallMaterial->SetScalarParameterValue(FName("Roughness"), 0.95f);
        WallMaterial->SetScalarParameterValue(FName("ConcreteScale"), 1.0f);
    }
}

void ABackroomsEnvironment::AddFluorescentLighting(int32 GridCols, int32 GridRows, float TileSize)
{
    // Place fluorescent light fixtures periodically
    for (int32 Y = 0; Y < GridRows; Y += 5)
    {
        for (int32 X = 0; X < GridCols; X += 5)
        {
            float LightX = X * TileSize + TileSize / 2;
            float LightY = Y * TileSize + TileSize / 2;
            float LightZ = CEILING_HEIGHT - 20.0f;

            ASpotLight* Light = GetWorld()->SpawnActor<ASpotLight>();
            if (Light)
            {
                Light->SetActorLocation(FVector(LightX, LightY, LightZ));
                Light->GetLightComponent()->SetIntensity(3000.0f);
                Light->GetLightComponent()->SetLightColor(FLinearColor(0.9f, 0.85f, 0.7f));
                Light->GetLightComponent()->SetAttenuationRadius(1500.0f);
                Light->GetLightComponent()->SetCastShadows(true);

                FluorescentLights.Add(Light);
            }
        }
    }
}

void ABackroomsEnvironment::GenerateQuad(TArray<FVector>& Vertices, TArray<int32>& Triangles, TArray<FVector>& Normals,
    TArray<FVector2D>& UVs, FVector P1, FVector P2, FVector P3, FVector P4)
{
    int32 V0 = Vertices.Num();

    Vertices.Add(P1);
    Vertices.Add(P2);
    Vertices.Add(P3);
    Vertices.Add(P4);

    Triangles.Add(V0);
    Triangles.Add(V0 + 1);
    Triangles.Add(V0 + 2);
    Triangles.Add(V0);
    Triangles.Add(V0 + 2);
    Triangles.Add(V0 + 3);

    FVector Normal = FVector::CrossProduct((P2 - P1), (P4 - P1)).GetSafeNormal();
    Normals.Add(Normal);
    Normals.Add(Normal);
    Normals.Add(Normal);
    Normals.Add(Normal);

    UVs.Add(FVector2D(0, 0));
    UVs.Add(FVector2D(1, 0));
    UVs.Add(FVector2D(1, 1));
    UVs.Add(FVector2D(0, 1));
}