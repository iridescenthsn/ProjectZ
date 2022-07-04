// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "HitscanWeapons.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AHitscanWeapons : public AWeaponBase
{
	GENERATED_BODY()

protected:

	virtual void Shoot() override;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly,Category="Tracer")
	TSubclassOf<class ATracer> TracerClass;
	
};
