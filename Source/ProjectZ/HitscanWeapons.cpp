// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanWeapons.h"

#include "Tracer.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void AHitscanWeapons::Shoot()
{
	FHitResult HitResult = CalculateShot();

	ATracer* Tracer = GetWorld()->SpawnActor<ATracer>(TracerClass,GunMesh->GetSocketLocation(FName("MuzzleFlash")),
		UKismetMathLibrary::FindLookAtRotation(GunMesh->GetSocketLocation(FName("MuzzleFlash")),HitResult.Location));


	//
	Tracer->SetLifeSpan(HitResult.Distance/Tracer->GetMovementComponent()->GetMaxSpeed());

	if (HitResult.bBlockingHit)
	{
		AddDamage(HitResult);
	}

	SpawnImpactEffect(HitResult);
}
