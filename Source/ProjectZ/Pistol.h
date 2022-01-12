// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API APistol : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	APistol();

	void WeaponFire();
};
