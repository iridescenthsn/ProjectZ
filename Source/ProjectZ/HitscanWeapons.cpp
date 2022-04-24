// Fill out your copyright notice in the Description page of Project Settings.


#include "HitscanWeapons.h"


void AHitscanWeapons::Shoot()
{
	FHitResult HitResult = CalculateShot();

	if (HitResult.bBlockingHit)
	{
		AddDamage(HitResult);
	}

	SpawnImpactEffect(HitResult);
}
