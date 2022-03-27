// Fill out your copyright notice in the Description page of Project Settings.

#include "PistolAmmoShell.h"
#include "GameFramework/ProjectileMovementComponent.h"


// Sets default values
APistolAmmoShell::APistolAmmoShell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->InitialSpeed = 3.f;
	ProjectileMovement->MaxSpeed = 3.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

// Called when the game starts or when spawned
void APistolAmmoShell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APistolAmmoShell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

