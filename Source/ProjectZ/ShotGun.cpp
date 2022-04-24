// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "FPS_Character.h"

AShotGun::AShotGun()
{
	bIsWeaponAuto = false;
}

void AShotGun::BeginPlay()
{
	Super::BeginPlay();

	AmmoData.Damage = 38;
	AmmoData.CriticalHitChance = 0;
}

void AShotGun::Shoot()
{
	for (size_t i = 0; i < NumberOfPallets; i++)
	{
		FHitResult HitResult = CalculateShot();

		if (HitResult.bBlockingHit)
		{
			AddDamage(HitResult);
		}

		SpawnImpactEffect(HitResult);
	}
}
