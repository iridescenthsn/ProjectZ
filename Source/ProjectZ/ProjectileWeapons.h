// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"	
#include "WeaponBase.h"			
#include "ProjectileWeapons.generated.h"

class AProjectile;
class AProjectile_Tracer;

/**
 * 
 */
UCLASS()
class PROJECTZ_API AProjectileWeapons : public AWeaponBase
{
	GENERATED_BODY()
	
protected:

	virtual void Shoot() override;

	UPROPERTY(EditAnyWhere,BlueprintReadWrite,Category="Projectile")
	TSubclassOf<AProjectile> Projectile_BP;

};
