// Fill out your copyright notice in the Description page of Project Settings.

#include "AutomaticWeapons.h"
#include "FPS_Character.h"
#include "PistolAmmoShell.h"	
#include "Kismet/GameplayStatics.h"


AAutomaticWeapons::AAutomaticWeapons()
{
	bIsWeaponAuto = true;
}

void AAutomaticWeapons::BeginPlay()
{
	Super::BeginPlay();
	AmmoData.Damage = 100.f;
	AmmoData.CriticalHitChance = 10;
}

void AAutomaticWeapons::Tick(float DeltaTime)
{
	AActor::Tick(DeltaTime);

	if (RecoilTimelineDirection==ETimelineDirection::Forward)
	{
		RecoilTimeLine.TickTimeline(DeltaTime);
		RecoilFrameCount++;
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
	if (Player->bIsNearWall)
	{
		StopFire();
		return;
	}

	Super::WeaponFire();

	if (Player->bCanFire)
	{
		bIsWeaponFiring = true;
		GetWorldTimerManager().SetTimer(AutoFireHandle, this, &AAutomaticWeapons::AutoFire, AutomaticFireRate, true);
		UE_LOG(LogTemp,Warning,TEXT("WeaponFire"))
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

		bIsRecoilReset = false;
		GetWorldTimerManager().SetTimer(RecoilResetHandle, this, &AAutomaticWeapons::ResetRecoilTimeline, ResetRecoilRate, false);

		bIsWeaponFiring = false;
		GetWorldTimerManager().ClearTimer(AutoFireHandle);
	}
}


//starts playing the recoil timeline
void AAutomaticWeapons::StartPlayingTimeLine()
{
	RecoilTimelineDirection = ETimelineDirection::Forward;

	if (bIsRecoilReset)
	{
		RecoilAllAddedPitch = 0.0f;
		RecoilAllAddedYaw = 0.0f;
		PitchPullDown = 0.0f;
		YawPullDown = 0.0f;
		RecoilFrameCount = 0;
		PlayerPitchInput = 0.0f;
		PlayerYawInput = 0.0f;
		
		UE_LOG(LogTemp,Warning,TEXT("Yaw set to zero"))

		RecoilTimeLine.PlayFromStart();
	}
	else
	{
		RecoilTimeLine.Play();
	}
}

void AAutomaticWeapons::ResetRecoilTimeline()
{
	bIsRecoilReset = true;
}

void AAutomaticWeapons::AddRecoilPitch(float value)
{
	if (RecoilTimelineDirection == ETimelineDirection::Forward)
	{
		RecoilAllAddedPitch += -value;
		Player->AddControllerPitchInput(-value);
	}
	else
	{
		Player->AddControllerPitchInput(-PitchPullDown);
	}
}

void AAutomaticWeapons::AddRecoilYaw(float value)
{
	if (RecoilTimelineDirection == ETimelineDirection::Forward)
	{
		RecoilAllAddedYaw += value;
		Player->AddControllerYawInput(value);
	}
	else
	{
		Player->AddControllerYawInput(-YawPullDown);
	}
}

void AAutomaticWeapons::RevertRecoil()
{
	/*
	If same directions then ignore player input

	If opposite directions

		If player input abs is less than recoil abs then add them together

		If player input abs is more than or equal to recoil abs then dont pull 
	*/

	if (FMath::Sign(PlayerPitchInput)==FMath::Sign(RecoilAllAddedPitch))
	{
		PitchPullDown = RecoilAllAddedPitch / RecoilFrameCount;
	}
	else
	{
		if (FMath::Abs(PlayerPitchInput)<FMath::Abs(RecoilAllAddedPitch))
		{
			RecoilAllAddedPitch += PlayerPitchInput;
			PitchPullDown = RecoilAllAddedPitch / RecoilFrameCount;
		}
		else
		{
			PitchPullDown = 0;
		}
	}
	

	if (FMath::Sign(PlayerYawInput) == FMath::Sign(RecoilAllAddedYaw))
	{
		YawPullDown = RecoilAllAddedYaw / RecoilFrameCount;
	}
	else
	{
		if (FMath::Abs(PlayerPitchInput) < FMath::Abs(RecoilAllAddedPitch))
		{
			RecoilAllAddedYaw += PlayerYawInput;
			YawPullDown = RecoilAllAddedYaw / RecoilFrameCount;
		}
		else
		{
			YawPullDown = 0;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Yaw Pulldown : %f"),YawPullDown)

	RecoilTimelineDirection = ETimelineDirection::Backward;
	RecoilTimeLine.Reverse();
}




