// Fill out your copyright notice in the Description page of Project Settings.

#include "PistolAmmoShell.h"	
#include "AssaultRifle.h"

AAssaultRifle::AAssaultRifle()
{
	WeaponType = EWeaponType::AssaultRifle;
	SocketName = FName(TEXT("AssualtRifle_Socket"));
	//AmmoData->Damage = 100.f;
	//AmmoData->CriticalHitChance = 10;
}

void AAssaultRifle::WeaponFire()
{
	Super::WeaponFire();
	GunMesh->PlayAnimation(FireAnimation, false);

	AmmoShellEject();
}

void AAssaultRifle::AmmoShellEject()
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			const FRotator SpawnRotation = GunMesh->GetSocketRotation(FName(TEXT("AmmoEject")));
			const FVector SpawnLocation = GunMesh->GetSocketLocation(FName(TEXT("AmmoEject")));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


			World->SpawnActor<APistolAmmoShell>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
}
