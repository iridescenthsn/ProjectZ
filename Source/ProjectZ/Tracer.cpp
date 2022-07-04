// Fill out your copyright notice in the Description page of Project Settings.


#include "Tracer.h"

#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ATracer::ATracer()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot=CreateDefaultSubobject<USceneComponent>(FName("SceneRoot"));
	SetRootComponent(SceneRoot);

	MovementComponent=CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile movement component"));

	TracerMesh=CreateDefaultSubobject<UStaticMeshComponent>(FName("Tracer"));
	TracerMesh->SetupAttachment(SceneRoot);
	TracerMesh->CastShadow=false;
}

// Called when the game starts or when spawned
void ATracer::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATracer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

