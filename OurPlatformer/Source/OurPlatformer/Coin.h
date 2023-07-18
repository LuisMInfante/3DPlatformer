// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

class UStaticMeshComponent;
class URotatingMovementComponent;
class USphereComponent;
class UNiagaraSystem;

UCLASS()
class OURPLATFORMER_API ACoin : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACoin();

protected:

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    URotatingMovementComponent* RotatingMovementComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    USphereComponent* ColliderComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UNiagaraSystem* PickupEffect;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* PickupSound;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    float VolumeMultiplier {0.5};

    UPROPERTY(EditDefaultsOnly, Category = "VFX")
    float PickupEffectSpawnOffset {90};

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
