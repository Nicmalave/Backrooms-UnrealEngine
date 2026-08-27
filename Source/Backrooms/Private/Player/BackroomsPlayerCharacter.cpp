#include "Player/BackroomsPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"

ABackroomsPlayerCharacter::ABackroomsPlayerCharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    // Don't rotate character with camera
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 1200.f;

    // Create camera boom (pulls in towards the player if there's a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = true;

    // Create follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;
}

void ABackroomsPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();

    // Add Input Mapping Context
    if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            PlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void ABackroomsPlayerCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    // Update battery
    if (bFlashlightOn)
    {
        float DrainRate = BatteryDrainRate;
        if (bIsSprinting) DrainRate += 0.4f;
        ModifyBattery(-DrainRate * DeltaTime);

        if (BatteryPercent <= 0.0f)
        {
            bFlashlightOn = false;
        }
    }
    else
    {
        ModifyBattery(BatteryRechargeRate * DeltaTime);
    }
}

void ABackroomsPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInputComponent = FindComponentByClass<UEnhancedInputComponent>())
    {
        // Moving
        if (MoveAction)
        {
            EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABackroomsPlayerCharacter::Move);
        }

        // Looking
        if (LookAction)
        {
            EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABackroomsPlayerCharacter::Look);
        }

        // Sprinting
        if (SprintAction)
        {
            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABackroomsPlayerCharacter::Sprint);
            EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABackroomsPlayerCharacter::StopSprinting);
        }

        // Flashlight
        if (FlashlightAction)
        {
            EnhancedInputComponent->BindAction(FlashlightAction, ETriggerEvent::Started, this, &ABackroomsPlayerCharacter::ToggleFlashlight);
        }
    }
}

void ABackroomsPlayerCharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Find forward direction
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);
        const FVector ForwardDirection = FRotator(0, YawRotation.Yaw, 0).GetForwardVector();
        AddMovementInput(ForwardDirection, MovementVector.Y);

        // Find right direction
        const FVector RightDirection = FRotator(0, YawRotation.Yaw, 0).GetRightVector();
        AddMovementInput(RightDirection, MovementVector.X);
    }
}

void ABackroomsPlayerCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // Add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}

void ABackroomsPlayerCharacter::Sprint(const FInputActionValue& Value)
{
    bIsSprinting = true;
    GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ABackroomsPlayerCharacter::StopSprinting()
{
    bIsSprinting = false;
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
}

void ABackroomsPlayerCharacter::ToggleFlashlight()
{
    if (BatteryPercent > 2.0f || !bFlashlightOn)
    {
        bFlashlightOn = !bFlashlightOn;
    }
}

void ABackroomsPlayerCharacter::ModifyBattery(float Amount)
{
    BatteryPercent = FMath::Clamp(BatteryPercent + Amount, 0.0f, MaxBattery);
}