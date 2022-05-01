// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ImpactEffect.h"
#include "GrenadeImpactEffect.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AGrenadeImpactEffect : public AImpactEffect
{
	GENERATED_BODY()
	
protected:

	FVector SpawnLocation;

	virtual void BeginPlay() override;


public:

	void initializeParams(FHitResult hit, bool IsUsingHit, const FVector& ExplosiveLocation = FVector::ZeroVector);

};
