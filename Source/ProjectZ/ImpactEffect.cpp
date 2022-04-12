// Fill out your copyright notice in the Description page of Project Settings.

#include "ImpactEffect.h"
#include "Components/DecalComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AImpactEffect::AImpactEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AImpactEffect::BeginPlay()
{
	Super::BeginPlay();

	DecalRotation = HitResult.Normal.Rotation();
	DecalRotation.Roll = FMath::RandRange(-180, 180);
	if (HitResult.PhysMaterial==nullptr)
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

	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), EmitterToSpawn, HitResult.ImpactPoint);

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
	
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), SoundToPlay,HitResult.ImpactPoint);
	FVector DSize = FVector(DecalSize, DecalSize, DecalSize);
	switch (HitResult.PhysMaterial->SurfaceType)
	{

		UDecalComponent* Decal;

	case SurfaceType_Default:
	
		Decal = UGameplayStatics::SpawnDecalAttached
		(
			DefaultDecalMat,
			DSize,
			HitResult.GetComponent(),
			HitResult.BoneName,
			HitResult.ImpactPoint,
			DecalRotation,
			EAttachLocation::KeepWorldPosition,
			0.0f
		);
		Decal->SetFadeScreenSize(DecalFadeScreenSize);
		break;

	case SurfaceType1:
		Decal = UGameplayStatics::SpawnDecalAttached
		(
			ConcreteDecalMat,
			DSize,
			HitResult.GetComponent(),
			HitResult.BoneName,
			HitResult.ImpactPoint,
			DecalRotation,
			EAttachLocation::KeepWorldPosition,
			0.0f
		);
		Decal->SetFadeScreenSize(DecalFadeScreenSize);
		break;

	case SurfaceType2:
		break;

	default:
		break;
	}

	
	if (!bIsUsingHitResult)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DefaultFX, HitResult.ImpactPoint);
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DefaultSound, HitResult.ImpactPoint);
		UGameplayStatics::SpawnDecalAttached
		(
			DefaultDecalMat,
			DSize,
			HitResult.GetComponent(),
			HitResult.BoneName,
			HitResult.ImpactPoint,
			DecalRotation,
			EAttachLocation::KeepWorldPosition,
			3.0f
		);
	}
}

// Called every frame
void AImpactEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AImpactEffect::initialize(FHitResult hit, bool IsUsingHit)
{
	HitResult = hit;
	bIsUsingHitResult = IsUsingHit;
}

