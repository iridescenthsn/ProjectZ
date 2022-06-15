// Fill out your copyright notice in the Description page of Project Settings.

#include "AutomaticWeapons.h"
#include "FPS_Character.h"
#include "PistolAmmoShell.h"	


AAutomaticWeapons::AAutomaticWeapons()
{
	bIsWeaponAuto = true;
}

void AAutomaticWeapons::BeginPlay()
{
	Super::BeginPlay();
	AmmoData.Damage = 100.f;
	AmmoData.CriticalHitChance = 10;
	
	PerBulletRecoilPitch.SetNum(MaxAmmoInMag);
	PerBulletRecoilYaw.SetNum(MaxAmmoInMag);

	PerBulletRecoilPitch[0]= 0.1f;
	PerBulletRecoilPitch[1]= 0.08f;
	PerBulletRecoilPitch[2]= 0.06f;
	PerBulletRecoilPitch[3]= 0.04f;
	PerBulletRecoilPitch[4]= 0.04f;
	PerBulletRecoilPitch[5]= 0.04f;
	PerBulletRecoilPitch[6]= 0.03f;
	PerBulletRecoilPitch[7]= 0.02f;
	PerBulletRecoilPitch[8]= 0.01f;
	PerBulletRecoilPitch[9]= 0.01f;
	PerBulletRecoilPitch[10]= 0.03f;
	PerBulletRecoilPitch[11]= 0.03f;
	PerBulletRecoilPitch[12]= 0.01f;
	PerBulletRecoilPitch[13]= 0.01f;
	PerBulletRecoilPitch[14]= 0.01f;
	PerBulletRecoilPitch[15]= 0.01f;
	PerBulletRecoilPitch[16]= 0.01f;
	PerBulletRecoilPitch[17]= 0.01f;
	PerBulletRecoilPitch[18]= 0.01f;
	PerBulletRecoilPitch[19]= 0.01f;
	PerBulletRecoilPitch[20]= 0.01f;
	PerBulletRecoilPitch[21]= 0.02f;
	PerBulletRecoilPitch[22]= -0.03f;
	PerBulletRecoilPitch[23]= 0.01f;

	PerBulletRecoilYaw[0]= 0.02f;
	PerBulletRecoilYaw[1]= 0.02f;
	PerBulletRecoilYaw[2]= 0.02f;
	PerBulletRecoilYaw[3]= 0.02f;
	PerBulletRecoilYaw[4]= 0.02f;
	PerBulletRecoilYaw[5]= 0.01f;
	PerBulletRecoilYaw[6]= 0.01f;
	PerBulletRecoilYaw[7]= -0.01f;
	PerBulletRecoilYaw[8]= -0.01f;
	PerBulletRecoilYaw[9]= -0.02f;
	PerBulletRecoilYaw[10]= -0.03f;
	PerBulletRecoilYaw[11]= -0.03f;
	PerBulletRecoilYaw[12]= -0.03f;
	PerBulletRecoilYaw[13]= -0.02f;
	PerBulletRecoilYaw[14]= -0.04f;
	PerBulletRecoilYaw[15]= -0.01f;
	PerBulletRecoilYaw[16]= -0.02f;
	PerBulletRecoilYaw[17]= -0.01f;
	PerBulletRecoilYaw[18]= 0.01f;
	PerBulletRecoilYaw[19]= 0.02f;
	PerBulletRecoilYaw[20]= 0.03f;
	PerBulletRecoilYaw[21]= 0.03f;
	PerBulletRecoilYaw[22]= 0.04f;
	PerBulletRecoilYaw[23]= 0.05f;

}

void AAutomaticWeapons::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);
	
	RecoilTimeLine.TickTimeline(DeltaTime);
	
}

//Call auto fire frequently based on fire rate for as long as holding the button down(sets timer)
void AAutomaticWeapons::WeaponFire()
{
	if (Player->bIsNearWall)
	{
		StopFire();
		return;
	}

	Super::WeaponFire();

	if (Player->bCanFire&&bIsReadyToFire)
	{
		bIsWeaponFiring = true;

		//dont pull weapon up(animation) if adsing
		if (!Player->IsADSing)
		{
			Player->CharacterFireWeapon.Broadcast(WeaponType);
		}
		else
		{
			Player->CharacterStopFireWeapon.Broadcast();
		}

		bIsReadyToFire=false;
		GetWorldTimerManager().SetTimer(AutoFireHandle, this, &AAutomaticWeapons::AutoFire, FireRate, false);
	}
}

void AAutomaticWeapons::AutoFire()
{
	bIsReadyToFire=true;
	if (bIsWeaponFiring)
	{
		WeaponFire();
	}
}

//Stops calling the auto fire function when releasing the firing button(clears timer)
void AAutomaticWeapons::StopFire()
{
	if (bIsWeaponFiring)
	{
		Player->CharacterStopFireWeapon.Broadcast();

		CalculateReverseRecoil();

		GetWorldTimerManager().SetTimer(RecoilReverseHandle, this, &AAutomaticWeapons::RevertRecoil, RecoilReverseSpeed, true);
		
		GetWorldTimerManager().SetTimer(RecoilResetHandle, this, &AAutomaticWeapons::ResetRecoil, ResetRecoilRate, true);

		bIsWeaponFiring = false;
		Player->bCanFire=true;
		//GetWorldTimerManager().ClearTimer(AutoFireHandle);
	}
}






