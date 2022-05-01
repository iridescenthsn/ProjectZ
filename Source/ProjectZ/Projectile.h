// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponBase.h"
#include "Projectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class AImpactEffect;
class USceneComponent;

UCLASS()
class PROJECTZ_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Sphere")
	USphereComponent* Sphere;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SceneRoot")
	USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FAmmoData AmmoData;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float CriticalHitModifier;

	UFUNCTION()
	virtual void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	virtual void AddDamage(const FHitResult &Hit);

	UFUNCTION()
	void SpawnImpactEffect(const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf <AImpactEffect> ImpactEffectBP;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UStaticMeshComponent* GetMesh() const { return Mesh; }
};
