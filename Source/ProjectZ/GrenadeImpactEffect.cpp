// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeImpactEffect.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"

void AGrenadeImpactEffect::BeginPlay()
{
	AActor::BeginPlay();

	if (HitResult.PhysMaterial == nullptr)
	{
		return;
	}
	UParticleSystem* EmitterToSpawn;
	switch (HitResult.PhysMaterial->SurfaceType)
	{
	case SurfaceType_Default:
		EmitterToSpawn = DefaultFX;
		break;

	case SurfaceType1:
		EmitterToSpawn = ConcreteFX;
		break;

	case SurfaceType2:

	case SurfaceType3:

	case SurfaceType4:

	case SurfaceType5:

	case SurfaceType6:
		EmitterToSpawn = FleshFX;
		break;

	default:
		EmitterToSpawn = DefaultFX;
		break;
	}

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterToSpawn, SpawnLocation);

	USoundBase* SoundToPlay;
	switch (HitResult.PhysMaterial->SurfaceType)
	{
	case SurfaceType_Default:
		SoundToPlay = DefaultSound;
		break;

	case SurfaceType1:
		SoundToPlay = ConcreteSound;
		break;

	case SurfaceType2:

	case SurfaceType3:

	case SurfaceType4:

	case SurfaceType5:

	case SurfaceType6:
		SoundToPlay = FleshSound;
		break;

	default:
		SoundToPlay = DefaultSound;
		break;
	}

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay,SpawnLocation);


	if (!bIsUsingHitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultFX, SpawnLocation);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DefaultSound, SpawnLocation);
	}
}

void AGrenadeImpactEffect::initializeParams(FHitResult hit, bool IsUsingHit, const FVector& ExplosiveLocation)
{
	HitResult = hit;
	bIsUsingHitResult = IsUsingHit;
	SpawnLocation = ExplosiveLocation;
}
