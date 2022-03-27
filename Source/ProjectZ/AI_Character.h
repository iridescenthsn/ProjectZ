// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "WeaponBase.h"
#include "TakeDamage.h"
#include "AI_Character.generated.h"

UCLASS()
class PROJECTZ_API AAI_Character : public ACharacter, public ITakeDamage
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAI_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IsDead")
	bool bIsDead;


	//Calculates the taken damage
	UFUNCTION()
	float SetDamage(float Damage,float CriticalHitChance, float CriticalHitModifier, FHitResult HitResult);

	//Updates health and returns true if character is dead
	UFUNCTION()
	bool UpdateHealth(float Damage);

	//Gets called when damage is applied
	void TakeDamage(FAmmoData AmmoData, float CriticalHitModifier, FHitResult HitResult) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
