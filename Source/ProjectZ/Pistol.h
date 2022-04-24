// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitscanWeapons.h"
#include "Pistol.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API APistol : public AHitscanWeapons
{
	GENERATED_BODY()
	
public:
	APistol();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
