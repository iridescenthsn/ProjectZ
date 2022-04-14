// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotGun.h"
#include "FPS_Character.h"

AShotGun::AShotGun()
{
	WeaponType = EWeaponType::ShotGun;
	SocketName = FName(TEXT("ShotGun_Socket"));
	bIsWeaponAuto = false;
}

void AShotGun::BeginPlay()
{
	Super::BeginPlay();

	AmmoData.Damage = 38;
	AmmoData.CriticalHitChance = 0;
}

void AShotGun::WeaponFire()
{
	if (Player->bCanFire)
	{
		if (MagStatus().bHasAmmo)
		{
			bIsWeaponFiring = true;

			Player->CharacterFireWeapon.Broadcast(WeaponType);

			CurrentAmmoInMag--;

			GunMesh->PlayAnimation(FireAnimation, false);

			for (int32 i = 1; i <= NumberOfPallets; i++)
			{
				FHitResult HitResult = CalculateShot();

				SpawnDecal(HitResult);
			}
		}
		else
		{
			StopFire();
			Player->bCanFire = false;

			if (HasReservedAmmo())
			{
				Player->Reload();
			}
		}
	}
}

