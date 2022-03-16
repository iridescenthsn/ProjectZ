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

	UPROPERTY(EditDefaultsOnly, Category = "AmmoShell")
	TSubclassOf<class APistolAmmoShell> ProjectileClass;

	void WeaponFire();

	void AmmoShellEject();
};
