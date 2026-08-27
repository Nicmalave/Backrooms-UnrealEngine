#include "Graphics/BackroomsPostProcessing.h"
#include "Camera/PlayerCameraManager.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Math/UnrealMathUtility.h"

ABackroomsPostProcessing::ABackroomsPostProcessing()
{
    PrimaryActorTick.bCanEverTick = true;
}

void ABackroomsPostProcessing::BeginPlay()
{
    Super::BeginPlay();
}

void ABackroomsPostProcessing::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update screen shake
    if (ShakeDuration > 0.0f)
    {
        ShakeDuration -= DeltaTime;
        if (ShakeDuration <= 0.0f)
        {
            CurrentShakeAmount = 0.0f;
        }
    }

    // Update sanity pulse effect
    SanityPulseIntensity = FMath::Max(0.0f, SanityPulseIntensity - DeltaTime * 2.0f);
}

void ABackroomsPostProcessing::ApplyScreenShake(float Intensity, float Duration)
{
    CurrentShakeAmount = Intensity;
    ShakeDuration = Duration;

    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (PlayerController->PlayerCameraManager)
        {
            PlayerController->PlayerCameraManager->StartShake(
                UCameraShake::StaticClass(),
                Intensity);
        }
    }
}

void ABackroomsPostProcessing::ApplyVignette(float Intensity)
{
    VignetteIntensity = FMath::Clamp(Intensity, 0.0f, 1.0f);
}

void ABackroomsPostProcessing::PlayFlash()
{
    // Trigger brief screen flash for level transitions
    if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        PlayerController->PlayerCameraManager->StartShake(
            UCameraShake::StaticClass(),
            0.1f);
    }
}

void ABackroomsPostProcessing::PulseSanityEffect(float Sanity)
{
    // Intensify visual distortion as sanity decreases
    float SanityRatio = FMath::Clamp(Sanity / 100.0f, 0.0f, 1.0f);
    
    if (SanityRatio < 0.3f)
    {
        SanityPulseIntensity = (0.3f - SanityRatio) * 2.0f;
    }

    // Apply screen shake proportional to low sanity
    if (SanityRatio < 0.2f)
    {
        ApplyScreenShake(0.5f * (1.0f - SanityRatio), 0.1f);
    }
}