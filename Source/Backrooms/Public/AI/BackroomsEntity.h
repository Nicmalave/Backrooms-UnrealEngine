#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BackroomsEntity.generated.h"

class ABackroomsPlayerCharacter;
class USkeletalMeshComponent;
class UAnimInstance;

UENUM(BlueprintType)
enum class EEntityType : uint8
{
    ET_Wretch UMETA(DisplayName = "Wretch"),
    ET_Hound UMETA(DisplayName = "Hound"),
    ET_Smiler UMETA(DisplayName = "Smiler")
};

UENUM(BlueprintType)
enum class EEntityState : uint8
{
    ES_Wander UMETA(DisplayName = "Wander"),
    ES_Alert UMETA(DisplayName = "Alert"),
    ES_Chase UMETA(DisplayName = "Chase")
};

USTRUCT(BlueprintType)
struct FEntityStats
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float ChaseSpeed = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float SenseRadius = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float VisibilityThreshold = 1.1f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float HeightOffset = 0.0f;
};

UCLASS()
class BACKROOMS_API ABackroomsEntity : public APawn
{
    GENERATED_BODY()

public:
    ABackroomsEntity();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EEntityType EntityType = EEntityType::ET_Wretch;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    EEntityState CurrentState = EEntityState::ES_Wander;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    FEntityStats EntityStats;

    UFUNCTION(BlueprintCallable, Category = "AI")
    void DetectPlayer(ABackroomsPlayerCharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void ChasePlayer(ABackroomsPlayerCharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void WanderAround();

    UFUNCTION(BlueprintCallable, Category = "AI")
    bool CanSeePlayer(ABackroomsPlayerCharacter* Player) const;

private:
    UPROPERTY()
    ABackroomsPlayerCharacter* TargetPlayer;

    UPROPERTY()
    USkeletalMeshComponent* SkeletalMesh;

    FVector WanderTarget = FVector::ZeroVector;
    float AlertTimer = 0.0f;
    float TeleportCooldown = 0.0f;
    bool bCanSeePlayer = false;

    // Detection logic per entity type
    void UpdateWretchBehavior(float DeltaTime);
    void UpdateHoundBehavior(float DeltaTime);
    void UpdateSmilerBehavior(float DeltaTime);
};