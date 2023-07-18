// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerCharacter.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Components/WidgetComponent.h"

// Sets default values
APlatformerCharacter::APlatformerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Character Movement
	GetCharacterMovement()->GravityScale = DefaultGravity;
	GetCharacterMovement()->JumpZVelocity = JumpStrengthLevels[0];
	GetCharacterMovement()->bOrientRotationToMovement = true;

	bUseControllerRotationYaw = false;

	OverheadWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidget"));
	OverheadWidget->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APlatformerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		PlatformerPlayerController = PlayerController;

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(PlatformerMappingContext, 0);
		}
	}	
}

void APlatformerCharacter::Move(const FInputActionValue& Value) 
{
	// Get input value
	const FVector2D MovementVector = Value.Get<FVector2D>();
	if (PlatformerPlayerController)
	{
		// Find forward direction
		const FRotator Rotation = PlatformerPlayerController->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		// Forward Vector corresponding to the direction the CONTROLLER is pointing in
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, MovementVector.Y);
		
		// Right Vector
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlatformerCharacter::Look(const FInputActionValue& Value) 
{
	// Get Input Value
	const FVector2D LookDirection = Value.Get<FVector2D>();

	if(PlatformerPlayerController)
	{
		AddControllerYawInput(LookDirection.X * LookSpeed * GetWorld()->GetDeltaSeconds());
		AddControllerPitchInput(LookDirection.Y * LookSpeed * GetWorld()->GetDeltaSeconds());
	}
}

void APlatformerCharacter::AdjustJumpStrength() 
{
	switch (JumpCounter)
	{
		case 1:
			GetCharacterMovement()->JumpZVelocity = JumpStrengthLevels[1];
			break;
		case 2:
			GetCharacterMovement()->JumpZVelocity = JumpStrengthLevels[2];
			break;
		default:
			break;
	}
}

void APlatformerCharacter::ResetJumpStrength() 
{
	JumpCounter = 0;
	GetCharacterMovement()->JumpZVelocity = JumpStrengthLevels[0];
}

void APlatformerCharacter::WallJump(FHitResult HitResult) 
{
	UE_LOG(LogTemp, Display, TEXT("Wall Jumped"));
	DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 10.f, 12, FColor::Red, false, 5.f);

    // Calculate the wall jump direction by reflecting the character's forward vector around the wall's normal.
    FVector WallNormal = HitResult.Normal;
    FVector CharacterForward = GetActorForwardVector();
    FVector WallJumpDirection = FVector::VectorPlaneProject(CharacterForward, WallNormal) - CharacterForward;

	// Debug: Draw the vector plane (the projected vector) as a green line.
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector::VectorPlaneProject(CharacterForward, WallNormal) * 100.0f, FColor::Green, false, 5.0f);

    // Debug: Draw the reflected vector as a blue line.
    DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + FVector::VectorPlaneProject(CharacterForward, WallNormal) - CharacterForward * 100.0f, FColor::Blue, false, 5.0f);

    // Set the character's velocity to achieve a consistent upward velocity for the wall jump.
    float WallJumpVelocity = 1000.0f; 
    FVector NewVelocity = WallJumpDirection.GetSafeNormal() * WallJumpVelocity;
    NewVelocity.Z = WallJumpVelocity + 100.f; // Set the upward velocity to WallJumpVelocity + a lil more.

    // Set the character's velocity directly.
    GetCharacterMovement()->Velocity = NewVelocity;

	// Rotate the character 180 degrees after the wall jump.
    FRotator CurrentRotation = GetActorRotation();
    FRotator TargetRotation = CurrentRotation + FRotator(0.0f, 180.0f, 0.0f);
    SetActorRotation(TargetRotation);
}

void APlatformerCharacter::Jump() 
{
    // Perform a line trace to check if the character is touching a wall while jumping.
    FVector Start = GetActorLocation();
    FVector Direction = GetActorForwardVector(); // Check if character is facing a wall
    float TraceLength = 50.0f;
    FVector End = Start + Direction * TraceLength;

    FHitResult HitResult;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this); // Ignore the character itself in the line trace

    // If the line trace hits a wall and the character is falling, execute the wall jump.
    if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_WorldStatic, Params) && GetCharacterMovement()->IsFalling())
    {
		WallJump(HitResult); 
        return; // Exit the Jump function to prevent regular jumping behavior.
    }
	
    // Do a regular jump if not wall jumping
	Super::Jump();
}

void APlatformerCharacter::OnJumped_Implementation() 
{
	// notified every jump
	GetCharacterMovement()->bNotifyApex =  true;
	JumpCounter++;
	GetWorldTimerManager().ClearTimer(JumpResetTimer);

	Super::OnJumped_Implementation();
}

void APlatformerCharacter::NotifyJumpApex() 
{
	// Increase velocity on the way down (from apex)
	GetCharacterMovement()->GravityScale = FallingGravity; 
	
	Super::NotifyJumpApex();
}

void APlatformerCharacter::Landed(const FHitResult& Hit) 
{
	GetCharacterMovement()->GravityScale = DefaultGravity;
	if (JumpCounter > 2)
	{
		ResetJumpStrength();
	}

	AdjustJumpStrength();
	GetWorldTimerManager().SetTimer(JumpResetTimer, this, &APlatformerCharacter::ResetJumpStrength, JumpTimerDelay, false);

	Super::Landed(Hit);
}

// Called every frame
void APlatformerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlatformerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if(UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Movement
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlatformerCharacter::Move);

		// Viewing
		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlatformerCharacter::Look);

		// Jumping
		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlatformerCharacter::Jump);
		// Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}

}

