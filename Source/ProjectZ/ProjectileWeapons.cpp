
// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileWeapons.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"

void AProjectileWeapons::Shoot()
{
	FHitResult HitResult = CalculateShot();

	FVector Tracer_SpawnLocation = GetGunMesh()->GetSocketLocation(FName(TEXT("MuzzleFlash")));

	FActorSpawnParameters ProjectileSpawnParameters;
	ProjectileSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(Projectile_BP, HitResult.TraceStart, Camera->GetComponentRotation(), ProjectileSpawnParameters);

	Projectile->GetMesh()->SetWorldLocation(Tracer_SpawnLocation);
	Projectile->GetMesh()->SetHiddenInGame(false);
	
}
