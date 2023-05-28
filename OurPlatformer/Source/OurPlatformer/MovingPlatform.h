// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingPlatform.generated.h"

UCLASS()
class OURPLATFORMER_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMovingPlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Timer Handle for reversing platform
	FTimerHandle ReverseTimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	FVector StartLocation;

	FVector PreviousVelocity;

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	FVector PlatformVelocity = FVector(0, 100, 0);

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	float TravelDistance = 1000;

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	FRotator RotationVelocity = FRotator(0, 0, 0);

	UPROPERTY(EditAnywhere, Category = "Moving Platform")
	bool ShouldPause = false;

	void MovePlatform(float DeltaTime);

	void StopPlatform(FVector UnitVector);

	void ReversePlatform();

	void RotatePlatform(float DeltaTime);

	bool ShouldPlatformReturn() const;

	float GetDistanceTraveled() const;
};
