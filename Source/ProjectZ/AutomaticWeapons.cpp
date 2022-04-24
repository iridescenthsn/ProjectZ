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

	FTimerHandle RecoilTimerHandle;
	GetWorldTimerManager().SetTimer(RecoilTimerHandle, this, &AAutomaticWeapons::AdvanceTimeLine, 0.05f, true, 0);
}

void AAutomaticWeapons::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (RecoilTimelineDirection==ETimelineDirection::Forward)
	{
		NumberOfFramesToRevert++;
	}

	//Advance the timeline faster when reverting recoil
	if (RecoilTimelineDirection == ETimelineDirection::Backward)
	{
		for (size_t i = 0; i < ReverseTimeLineSpeed; i++)
		{
			RecoilTimeLine.TickTimeline(DeltaTime);
		}
	}
}

//Call auto fire frequently based on fire rate for as long as holding the button down(sets timer)
void AAutomaticWeapons::WeaponFire()
{
	Super::WeaponFire();

	if (Player->bCanFire)
	{
		bIsWeaponFiring = true;
		GetWorldTimerManager().SetTimer(AutoFireHandle, this, &AAutomaticWeapons::AutoFire, AutomaticFireRate, true);

		AmmoShellEject();
	}
}

void AAutomaticWeapons::AutoFire()
{
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
		Super::StopFire();

		bIsWeaponFiring = false;
		GetWorldTimerManager().ClearTimer(AutoFireHandle);
	}
}

void AAutomaticWeapons::AddRecoilPitch(float value)
{
	if (RecoilTimelineDirection == ETimelineDirection::Forward)
	{
		RecoilAllAdedPitch += value;
		Player->AddControllerPitchInput(-value);
	}
	else
	{
		Player->AddControllerPitchInput(PitchPullDown);
	}
}

void AAutomaticWeapons::RevertRecoil()
{
	if (-PlayerPitchInput > RecoilAllAdedPitch)
	{
		RecoilAllAdedPitch += PlayerPitchInput;
	}

	if (RecoilAllAdedPitch >= PlayerPitchInput)
	{
		RecoilAllAdedPitch -= PlayerPitchInput;
		PitchPullDown = RecoilAllAdedPitch / NumberOfFramesToRevert;

		RecoilTimelineDirection = ETimelineDirection::Backward;
		RecoilTimeLine.Reverse();
	}
}

void AAutomaticWeapons::AdvanceTimeLine()
{
	if (RecoilTimelineDirection==ETimelineDirection::Forward)
	{
		RecoilTimeLine.TickTimeline(0.05f);
	}
}



