#pragma once

#include "CoreMinimal.h"
#include "Engine/Actor.h"
#include "BackroomsPostProcessing.generated.h"

class APlayerCameraManager;

UCLASS()
class BACKROOMS_API ABackroomsPostProcessing : public AActor
{
    GENERATED_BODY()

public:
    ABackroomsPostProcessing();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

    // Visual effects
    UFUNCTION(BlueprintCallable, Category = "Visual Effects")
    void ApplyScreenShake(float Intensity, float Duration);

    UFUNCTION(BlueprintCallable, Category = "Visual Effects")
    void ApplyVignette(float Intensity);

    UFUNCTION(BlueprintCallable, Category = "Visual Effects")
    void PlayFlash();

    UFUNCTION(BlueprintCallable, Category = "Visual Effects")
    void PulseSanityEffect(float Sanity);

private:
    float CurrentShakeAmount = 0.0f;
    float ShakeDuration = 0.0f;
    float VignetteIntensity = 0.0f;
    float SanityPulseIntensity = 0.0f;
};