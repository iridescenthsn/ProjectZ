// Fill out your copyright notice in the Description page of Project Settings.

#include "Pistol.h"
#include "PistolAmmoShell.h"	

APistol::APistol()
{
	WeaponType = EWeaponType::Pistol;
	SocketName = FName(TEXT("Pistol_Socket"));
}

void APistol::WeaponFire()
{
	Super::WeaponFire();
	GunMesh->PlayAnimation(FireAnimation, false);

	// try and fire a projectile
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
