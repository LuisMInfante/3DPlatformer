// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformerAnimInstance.h"
#include "PlatformerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"


void UPlatformerAnimInstance::NativeInitializeAnimation() 
{
    Super::NativeInitializeAnimation();

    PlatformerCharacter = Cast<APlatformerCharacter>(TryGetPawnOwner());
}

void UPlatformerAnimInstance::NativeUpdateAnimation(float DeltaTime) 
{
    Super::NativeUpdateAnimation(DeltaTime);

    if (PlatformerCharacter == nullptr)
    {
        PlatformerCharacter = Cast<APlatformerCharacter>(TryGetPawnOwner());
    }

    if (PlatformerCharacter == nullptr)
    {
        return;
    }
    
    FVector Velocity = PlatformerCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    bIsAirborne = PlatformerCharacter->GetCharacterMovement()->IsFalling();
    bIsAccelerating = (PlatformerCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f) ? true : false;
}
