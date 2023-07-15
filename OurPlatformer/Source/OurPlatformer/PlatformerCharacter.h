// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlatformerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class OURPLATFORMER_API APlatformerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlatformerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Enhanced Input Mappings and Context
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* PlatformerMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* JumpAction;

	// InputActions
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	// Other Movement Functions
	void AdjustJumpStrength();
	void ResetJumpStrength();

	// Overrides
	virtual void OnJumped_Implementation() override; // called when character starts jumping
	virtual void NotifyJumpApex() override; // Called when character reaches apex of jump
	virtual void Landed(const FHitResult& Hit) override; // Called when character lands (triggers OnLanded)

	// Movement variables
	FTimerHandle JumpResetTimer;
	int32 JumpCounter = 0;
	float DefaultGravity = 2.8f;
	float FallingGravity = 5.0f;
	float JumpTimerDelay = 0.1f;
	float JumpStrengthLevels[3] = {1000.f, 1400.f, 2000.f};


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	APlayerController* PlatformerPlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LookSpeed = 25.f;


};
