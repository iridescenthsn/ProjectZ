// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RadialForce.generated.h"

UCLASS()
class PROJECTZ_API ARadialForce : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARadialForce();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
};
