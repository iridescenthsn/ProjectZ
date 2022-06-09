// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExplosiveProjectile.h"
#include "GrenadeLauncherProjectile.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AGrenadeLauncherProjectile : public AExplosiveProjectile
{
	GENERATED_BODY()
	
protected:

	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) override;

	UPROPERTY()
	FTimerHandle ExplsionHandle;

	FTimerDelegate ExplosionDelegate;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ActivationAngle;

	UPROPERTY(EditAnywhere, Category = "Projectile")
	float ExplosionTime;

	UFUNCTION()
	void Explode(const FHitResult& Hit);

	UFUNCTION()
	void AddDamageAtLocation(const FHitResult& Hit, const FVector& ExplosiveLocation = FVector::ZeroVector);

	UFUNCTION()
	void SpawnImpactEffectAtLocation(const FHitResult& HitResult,const FVector& ExplosiveLocation = FVector::ZeroVector) const;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf <class AGrenadeImpactEffect> GrenadeImpactEffectBP;

	UPROPERTY(EditDefaultsOnly, Category = "Bounce")
	USoundBase* BounceSound;

	UPROPERTY(EditAnywhere, Category = "Bounce")
	float BounceVolumeMultiplier=1;
};
