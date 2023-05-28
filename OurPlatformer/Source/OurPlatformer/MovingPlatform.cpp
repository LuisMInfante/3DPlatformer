// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

// Sets default values
AMovingPlatform::AMovingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	StartLocation = GetActorLocation(); // Get platform's starting location
	PreviousVelocity = PlatformVelocity; // Store velocity for reversal
}

// Called every frame
void AMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MovePlatform(DeltaTime);

	RotatePlatform(DeltaTime);
}

void AMovingPlatform::MovePlatform(float DeltaTime)
{
	// Reverse the platform if it crossed distance threshold
	if(ShouldPlatformReturn())
	{
		FVector UnitVector = PlatformVelocity.GetSafeNormal();

		// Check if platform should stop
		if (ShouldPause)
		{
			StopPlatform(UnitVector);

			// Reverse Platform after delay
			GetWorldTimerManager().SetTimer(ReverseTimerHandle, this, &AMovingPlatform::ReversePlatform, PauseLength, false);
			return;
		}

		ReversePlatform();
	}

	// Move the platform
	else
	{
		FVector CurrentLocation = GetActorLocation();
		CurrentLocation += PlatformVelocity * DeltaTime;
		SetActorLocation(CurrentLocation);
	}
}

void AMovingPlatform::StopPlatform(FVector UnitVector)
{
	// Set new starting location based on unit vector (Avoids over-shooting)
	StartLocation += UnitVector * TravelDistance;

	// Set platform to new start location
	SetActorLocation(StartLocation); 

	// Save original velocity and stop the platform
	PreviousVelocity = PlatformVelocity;
	PlatformVelocity *= 0;
}

void AMovingPlatform::ReversePlatform()
{
	PlatformVelocity = -PreviousVelocity;
}

void AMovingPlatform::RotatePlatform(float DeltaTime)
{
	// Rotate platform (frame rate independent)
	AddActorLocalRotation(RotationVelocity * DeltaTime);
}

bool AMovingPlatform::ShouldPlatformReturn() const
{
	// Reverse platform if it passed Distance Traveled threshold
	return GetDistanceTraveled() > TravelDistance;
}

float AMovingPlatform::GetDistanceTraveled() const
{
	// Check how far platform has moved from start location
	return FVector::Distance(StartLocation, GetActorLocation());
}

