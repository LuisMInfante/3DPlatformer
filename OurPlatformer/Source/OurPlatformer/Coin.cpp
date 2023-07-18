// Fill out your copyright notice in the Description page of Project Settings.


#include "Coin.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/EngineTypes.h"
#include "NiagaraFunctionLibrary.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	ColliderComponent = CreateDefaultSubobject<USphereComponent>(TEXT("ColliderComponent"));
	ColliderComponent->SetupAttachment(RootComponent);
	ColliderComponent->SetGenerateOverlapEvents(true);
	ColliderComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ColliderComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	ColliderComponent->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnOverlapBegin);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	MeshComponent->SetupAttachment(RootComponent);
	MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	MeshComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	MeshComponent->SetGenerateOverlapEvents(false);

	RotatingMovementComponent = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovementComponent"));
}

void ACoin::OnOverlapBegin(
						UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
						int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	if (!Cast<ACharacter>(OtherActor))
	{
		return;
	}

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), VolumeMultiplier);
	}

	if (PickupEffect)
	{
		const FVector Offset = GetActorUpVector() * PickupEffectSpawnOffset;
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, PickupEffect, OtherActor->GetActorLocation() + Offset);
	}
	Destroy();
}

