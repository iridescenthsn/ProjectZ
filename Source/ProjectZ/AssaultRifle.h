// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "AssaultRifle.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AAssaultRifle : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	AAssaultRifle();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	FTimerHandle AutoFireHandle;

	void WeaponFire() override;
	void StopFire() override;

	virtual void AddRecoilPitch(float value) override;
	virtual void RevertRecoil() override;

	void AdvanceTimeLine();

	void AutoFire();
};
