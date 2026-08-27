#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "BackroomsPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ABackroomsFlashlight;

UCLASS()
class BACKROOMS_API ABackroomsPlayerCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    ABackroomsPlayerCharacter();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Flashlight system
    UFUNCTION(BlueprintCallable, Category = "Flashlight")
    void ToggleFlashlight();

    UFUNCTION(BlueprintCallable, Category = "Flashlight")
    bool IsFlashlightOn() const { return bFlashlightOn; }

    UFUNCTION(BlueprintCallable, Category = "Flashlight")
    void ModifyBattery(float Amount);

    UFUNCTION(BlueprintCallable, Category = "Flashlight")
    float GetBatteryPercent() const { return BatteryPercent; }

    // Movement
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float WalkSpeed = 600.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
    float SprintSpeed = 1200.0f;

private:
    // Camera
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    // Input system (Enhanced Input)
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* SprintAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
    UInputAction* FlashlightAction;

    // Flashlight
    bool bFlashlightOn = true;
    float BatteryPercent = 100.0f;
    float MaxBattery = 100.0f;
    float BatteryDrainRate = 1.15f;
    float BatteryRechargeRate = 2.2f;
    
    UPROPERTY()
    ABackroomsFlashlight* Flashlight;

    bool bIsSprinting = false;

    // Input callbacks
    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void Sprint(const FInputActionValue& Value);
    void StopSprinting();
};