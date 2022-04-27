// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Character.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AAI_Character::AAI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAI_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

float AAI_Character::SetDamage(float Damage, float CriticalHitChance, float CriticalHitModifier,const FHitResult& HitResult)
{
	float LocalDamage = Damage;
	bool bIsCriticalHit;

	if (CriticalHitChance >= FMath::RandRange(0,100))
	{
		bIsCriticalHit = true;
		LocalDamage *= CriticalHitModifier;
	}
	else
	{
		bIsCriticalHit = false;
	}

	LocalDamage = LocalDamage * HitResult.PhysMaterial->DestructibleDamageThresholdScale;
	return LocalDamage;
}

float AAI_Character::SetRadialDamage(float Damage, float Radius, const FHitResult& HitResult)
{
	float LocalDamage = Damage;

	float Distance = (GetActorLocation() - HitResult.ImpactPoint).Size();

	UE_LOG(LogTemp,Warning,TEXT("Distance/radius is : %f"),Distance/Radius)

	LocalDamage = (1 - (Distance / Radius)) * LocalDamage;
	LocalDamage = FMath::Clamp(LocalDamage,0.0f, Damage);

	return LocalDamage;
}

bool AAI_Character::UpdateHealth(float Damage)
{
	bool bLocalIsDead=false;
	CurrentHealth -= Damage;
	if (CurrentHealth<=0)
	{
		bLocalIsDead = true;
		CurrentHealth = 0;
	}
	return bLocalIsDead;
}

void AAI_Character::TakeDamage(const FAmmoData& AmmoData, float CriticalHitModifier,const FHitResult& HitResult)
{
	float DamageTaken = SetDamage(AmmoData.Damage, AmmoData.CriticalHitChance, CriticalHitModifier, HitResult);

	UE_LOG(LogTemp, Warning, TEXT("damage coming through : %f"),AmmoData.Damage)

	bIsDead = UpdateHealth(DamageTaken);
}

void AAI_Character::TakeRadialDamage(const FAmmoData& AmmoData, float CriticalHitModifier,const FHitResult& HitResult)
{
	float DamageTaken = SetRadialDamage(AmmoData.Damage,AmmoData.DamageRadius, HitResult);

	bIsDead = UpdateHealth(DamageTaken);
}

// Called every frame
void AAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

