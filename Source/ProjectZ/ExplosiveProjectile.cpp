// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosiveProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TakeDamage.h"
#include "PhysicsEngine/RadialForceComponent.h"


AExplosiveProjectile::AExplosiveProjectile()
{
	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("ExplsionForce"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

void AExplosiveProjectile::AddDamage(const FHitResult& Hit)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
	
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);

	TArray<AActor*> OutActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), Hit.ImpactPoint, AmmoData.DamageRadius, ObjectTypes, nullptr, IgnoreActors, OutActors);

	for (size_t i = 0; i < OutActors.Num(); i++)
	{
		AActor* HitActor = OutActors[i];
		if (HitActor)
		{
			ITakeDamage* TakeDamageInterface = Cast<ITakeDamage>(HitActor);
			if (TakeDamageInterface)
			{
				TakeDamageInterface->TakeRadialDamage(AmmoData, CriticalHitModifier, Hit,Hit.ImpactPoint);
			}
		}
	}
	
	ExplosionForce->FireImpulse();
}
