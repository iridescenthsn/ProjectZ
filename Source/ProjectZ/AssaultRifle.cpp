// Fill out your copyright notice in the Description page of Project Settings.

#include "AssaultRifle.h"
#include "FPS_Character.h"
#include "PistolAmmoShell.h"	


AAssaultRifle::AAssaultRifle()
{
	WeaponType = EWeaponType::AssaultRifle;
	SocketName = FName(TEXT("AssualtRifle_Socket"));
	bIsWeaponAuto = true;
}

void AAssaultRifle::BeginPlay()
{
	Super::BeginPlay();
	AmmoData.Damage = 100.f;
	AmmoData.CriticalHitChance = 10;
}

//Call auto fire frequently based on fire rate for as long as holding the button down(sets timer)
void AAssaultRifle::WeaponFire()
{
	Super::WeaponFire();

	if (Player->bCanFire)
	{
		GunMesh->PlayAnimation(FireAnimation, false);
		bIsWeaponFiring = true;
		GetWorldTimerManager().SetTimer(AutoFireHandle, this, &AAssaultRifle::AutoFire, AutomaticFireRate, true);

		AmmoShellEject();
	}
}

//Stops calling the auto fire function when releasing the firing button(clears timer)
void AAssaultRifle::StopFire()
{
	Super::StopFire();

	bIsWeaponFiring = false;
	GetWorldTimerManager().ClearTimer(AutoFireHandle);
}

void AAssaultRifle::AutoFire()
{
	if (bIsWeaponFiring)
	{
		WeaponFire();
	}
	
}


//Spawns an ammo shell on the ammo eject socket
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
