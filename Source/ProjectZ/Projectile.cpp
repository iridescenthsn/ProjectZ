// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "TakeDamage.h"
#include "ImpactEffect.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetSphereRadius(8.0f);
	Sphere->bReturnMaterialOnMove = true;
	Sphere->SetHiddenInGame(true);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Sphere);
	Mesh->SetHiddenInGame(true);
	Mesh->CastShadow = false;
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->ProjectileGravityScale = 1.0f;
	ProjectileMovement->bShouldBounce = true;

	Sphere->OnComponentHit.AddUniqueDynamic(this, &AProjectile::OnHit);

	this->InitialLifeSpan = 5.0f;

}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor!=this)
	{
		AddDamage(Hit);
		SpawnImpactEffect(Hit);
		Destroy();
	}
}

void AProjectile::AddDamage(const FHitResult& Hit)
{
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		ITakeDamage* TakeDamageInterface = Cast<ITakeDamage>(HitActor);
		if (TakeDamageInterface)
		{
			TakeDamageInterface->TakeDamage(AmmoData, CriticalHitModifier, Hit);
		}
	}
}

void AProjectile::SpawnImpactEffect(const FHitResult& HitResult)
{
	FTransform SpawnTransForm(FRotator(0, 0, 0), HitResult.ImpactPoint);
	AImpactEffect* ImpactEffect = Cast<AImpactEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ImpactEffectBP, SpawnTransForm));

	if (ImpactEffect != nullptr)
	{
		ImpactEffect->initialize(HitResult, HitResult.bBlockingHit);
		UGameplayStatics::FinishSpawningActor(ImpactEffect, SpawnTransForm);
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

