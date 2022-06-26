// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MeleeAttack.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"	
#include "TakeDamage.h"
#include "AI_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnTakeDamage,float,Damage,const FVector&,ImpactPoint);

UCLASS()
class PROJECTZ_API AAI_Character : public ACharacter, public ITakeDamage,public IMeleeAttack
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Character();

	UPROPERTY(BlueprintAssignable)
	FOnTakeDamage EventTakeDamage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;
 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IsDead")
	bool bIsDead;

	/** Distance of which the sphere trace will go */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float SphereDistanceMultiplier=50.0f;
	
	/** Sphere radius of damaging sphere trace */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float SphereRadius=25.0f;

	/** Damage the Ai applies on attack */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Damage")
	float AttackDamage=35.0f;

	//Calculates the taken damage
	UFUNCTION()
	float SetDamage(float Damage,float CriticalHitChance, float CriticalHitModifier,const FHitResult& HitResult) const;

	//Calculates the amount of Damage applied based on distance
	UFUNCTION()
	float SetRadialDamage(float Damage, float Radius,const FHitResult& HitResult, const FVector& ExplosiveLocation = FVector::ZeroVector) const;

	//Updates health and returns true if character is dead
	UFUNCTION()
	bool UpdateHealth(float Damage);

	//Gets called when damage is applied
	UFUNCTION()
	void TakeDamage(const FAmmoData& AmmoData, float CriticalHitModifier,const FHitResult& HitResult) override;

	//Gets called when radial damage is applied
	UFUNCTION()
	void TakeRadialDamage(const FAmmoData& AmmoData, float CriticalHitModifier,const FHitResult& HitResult, const FVector& ExplosiveLocation = FVector::ZeroVector) override;

	UFUNCTION()
	virtual void Melee() override;

	UFUNCTION()
	void AddMeleeDamage();

	void Die();

	UPROPERTY(EditDefaultsOnly,Category="Animation")
	UAnimMontage* AttackMontage;

	FTimerHandle AddDamageTimer;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
