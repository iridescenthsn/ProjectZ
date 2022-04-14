// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AShotGun : public AWeaponBase
{
	GENERATED_BODY()

	AShotGun();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 NumberOfPallets;

	virtual void WeaponFire() override;
};
