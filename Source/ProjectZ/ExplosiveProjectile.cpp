// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TakeDamage.h"

void AExplosiveProjectile::AddDamage(const FHitResult& Hit)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);

	TArray<AActor*> outActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Hit.ImpactPoint, SphereRadius, ObjectTypes, NULL, ignoreActors, outActors);

	for (size_t i = 0; i < outActors.Num(); i++)
	{
		AActor* HitActor = outActors[i];
		if (HitActor)
		{
			ITakeDamage* TakeDamageInterface = Cast<ITakeDamage>(HitActor);
			if (TakeDamageInterface)
			{
				TakeDamageInterface->TakeRadialDamage(AmmoData, CriticalHitModifier, Hit);
			}
		}
	}

	
}