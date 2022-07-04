// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanWeapons.h"

#include "Tracer.h"
#include "Kismet/KismetMathLibrary.h"


void AHitscanWeapons::Shoot()
{
	FHitResult HitResult = CalculateShot();

	//UKismetMathLibrary::FindLookAtRotation(GunMesh->GetSocketLocation(FName("MuzzleFlash")),HitResult.Location);

	GetWorld()->SpawnActor<ATracer>(TracerClass,GunMesh->GetSocketLocation(FName("MuzzleFlash")),
		UKismetMathLibrary::FindLookAtRotation(GunMesh->GetSocketLocation(FName("MuzzleFlash")),HitResult.Location));

	if (HitResult.bBlockingHit)
	{
		AddDamage(HitResult);
	}

	SpawnImpactEffect(HitResult);
}
