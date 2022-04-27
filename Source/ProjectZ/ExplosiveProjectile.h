// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "ExplosiveProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AExplosiveProjectile : public AProjectile
{
	GENERATED_BODY()

	virtual void AddDamage(const FHitResult& Hit) override;
	
protected:
	
	UPROPERTY(EditDefaultsOnly,Category="Radius")
	float SphereRadius;
};
