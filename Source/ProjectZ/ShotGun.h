// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitscanWeapons.h"
#include "ShotGun.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AShotGun : public AHitscanWeapons
{
protected:

	GENERATED_BODY()

	AShotGun();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 NumberOfPallets;

	virtual void Shoot() override;
};
