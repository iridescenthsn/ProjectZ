// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitscanWeapons.h"
#include "AutomaticWeapons.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AAutomaticWeapons : public AHitscanWeapons
{
	GENERATED_BODY()
	
public:
	AAutomaticWeapons();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	virtual void WeaponFire() override;
	virtual void StopFire() override;

	void AutoFire();
};
