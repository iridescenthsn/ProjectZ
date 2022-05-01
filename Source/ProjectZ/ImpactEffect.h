// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ImpactEffect.generated.h"

class UParticleSystem;
class USoundBase;
class UMaterial;

UCLASS()
class PROJECTZ_API AImpactEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AImpactEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="Impact effect")
	UParticleSystem* DefaultFX;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	USoundBase* DefaultSound;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	UMaterial* DefaultDecalMat;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	UParticleSystem* ConcreteFX;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	USoundBase* ConcreteSound;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	UMaterial* ConcreteDecalMat;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	UParticleSystem* FleshFX;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	USoundBase* FleshSound;

	UPROPERTY(EditDefaultsOnly, Category = "Impact effect")
	FRotator DecalRotation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Impact effect")
	float DecalFadeScreenSize;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Decal")
	float DecalSize = 3;

	FHitResult HitResult;
	bool bIsUsingHitResult;

	virtual void initialize(FHitResult hit, bool IsUsingHit);

};
