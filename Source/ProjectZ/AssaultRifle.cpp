// Fill out your copyright notice in the Description page of Project Settings.


#include "AssaultRifle.h"

AAssaultRifle::AAssaultRifle()
{
	WeaponType = EWeaponType::AssaultRifle;
	SocketName = FName(TEXT("AssualtRifle_Socket"));
}

void AAssaultRifle::WeaponFire()
{
	Super::WeaponFire();
	GunMesh->PlayAnimation(FireAnimation, false);
}
