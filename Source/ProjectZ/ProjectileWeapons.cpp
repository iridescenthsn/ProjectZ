
// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectileWeapons.h"
#include "Kismet/KismetMathLibrary.h"
#include "Camera/CameraComponent.h"
#include "Projectile.h"
#include "Components/SphereComponent.h"

void AProjectileWeapons::Shoot()
{
	const FVector Tracer_SpawnLocation = GetGunMesh()->GetSocketLocation(FName(TEXT("MuzzleFlash")));

	FActorSpawnParameters ProjectileSpawnParameters;
	ProjectileSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

	FRotator BulletSpreadRot;
	BulletSpreadRot.Pitch=Camera->GetComponentRotation().Pitch+ FMath::RandRange(-BulletSpread,+BulletSpread);
	BulletSpreadRot.Yaw=Camera->GetComponentRotation().Yaw +FMath::RandRange(-BulletSpread,+BulletSpread);
	BulletSpreadRot.Roll=Camera->GetComponentRotation().Roll;

	const AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(Projectile_BP, Camera->GetComponentLocation(),
		BulletSpreadRot, ProjectileSpawnParameters);

	Projectile->GetMesh()->SetWorldLocation(Tracer_SpawnLocation);
	Projectile->GetMesh()->SetHiddenInGame(false);
	
}
