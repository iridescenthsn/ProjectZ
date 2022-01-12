// Fill out your copyright notice in the Description page of Project Settings.


#include "Pistol.h"

APistol::APistol()
{
	WeaponType = EWeaponType::Pistol;
	SocketName = FName(TEXT("Pistol_Socket"));
}

void APistol::WeaponFire()
{
	Super::WeaponFire();
	GunMesh->PlayAnimation(FireAnimation, false);
}
