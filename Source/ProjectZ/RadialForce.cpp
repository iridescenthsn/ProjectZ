// Fill out your copyright notice in the Description page of Project Settings.


#include "RadialForce.h"

#include "PhysicsEngine/RadialForceComponent.h"

// Sets default values
ARadialForce::ARadialForce()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARadialForce::BeginPlay()
{
	Super::BeginPlay();
	
}


