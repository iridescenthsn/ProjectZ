// Fill out your copyright notice in the Description page of Project Settings.

#include "Pistol.h"
#include "PistolAmmoShell.h"	

APistol::APistol()
{
	WeaponType = EWeaponType::Pistol;
	SocketName = FName(TEXT("Pistol_Socket"));
	bIsWeaponAuto = false;
}

void APistol::BeginPlay()
{
	Super::BeginPlay();
	AmmoData.Damage = 100.f;
	AmmoData.CriticalHitChance = 10;
}



