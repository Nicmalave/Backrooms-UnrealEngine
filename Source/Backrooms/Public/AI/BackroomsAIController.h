#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "BackroomsAIController.generated.h"

class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UAISenseConfig_Hearing;

UCLASS()
class BACKROOMS_API ABackroomsAIController : public AAIController
{
    GENERATED_BODY()

public:
    ABackroomsAIController();
    virtual void BeginPlay() override;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAIPerceptionComponent* Perception;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAISenseConfig_Sight* SightConfig;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
    UAISenseConfig_Hearing* HearingConfig;

    UFUNCTION()
    void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

    UPROPERTY()
    AActor* CurrentTarget = nullptr;
};
