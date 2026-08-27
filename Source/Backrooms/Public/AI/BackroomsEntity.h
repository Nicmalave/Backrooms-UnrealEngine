#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BackroomsEntity.generated.h"

class ABackroomsPlayerCharacter;
class UBehaviorTree;
class UBlackboardComponent;

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

UCLASS()
class BACKROOMS_API ABackroomsEntity : public ACharacter
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

    // Behavior parameters
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float ChaseSpeed = 1500.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float SenseRadius = 2000.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
    float VisibilityThreshold = 1.1f;

    UFUNCTION(BlueprintCallable, Category = "AI")
    void ChasePlayer(ABackroomsPlayerCharacter* Player);

    UFUNCTION(BlueprintCallable, Category = "AI")
    void WanderAround();

private:
    UPROPERTY()
    ABackroomsPlayerCharacter* TargetPlayer;

    FVector WanderTarget;
    float AlertTimer = 0.0f;
    bool bCanSeePlayer = false;
};