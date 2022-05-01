// Fill out your copyright notice in the Description page of Project Settings.


#include "GrenadeLauncherProjectile.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TakeDamage.h"
#include "Kismet/GameplayStatics.h"
#include "GrenadeImpactEffect.h"
#include "DrawDebugHelpers.h"


void AGrenadeLauncherProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this)
	{
		float VectorDotProduct = FVector::DotProduct(Hit.ImpactNormal, GetActorForwardVector());
		float Angle = FMath::RadiansToDegrees(FMath::Acos(VectorDotProduct));

		UE_LOG(LogTemp, Warning, TEXT("Forward vector is %f"), Angle)

		if (Angle >= ActivationAngle)
		{
			Explode(Hit,GetActorLocation());
			UE_LOG(LogTemp, Warning, TEXT("Instant"))
		}
		else
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), BounceSound, Hit.ImpactPoint,BounceVolumeMultiplier);

			if (!GetWorldTimerManager().IsTimerActive(ExplsionHandle))
			{
				ExplosionDelegate.BindUFunction(this, FName(TEXT("Explode")), Hit, GetActorLocation());
				UE_LOG(LogTemp, Warning, TEXT("timer call"))
				GetWorldTimerManager().SetTimer(ExplsionHandle, ExplosionDelegate, ExplosionTime, false);
			}
		}
	}
}


void AGrenadeLauncherProjectile::Explode(const FHitResult& Hit, const FVector& ExplosiveLocation)
{
	AddDamageAtLocation(Hit,GetActorLocation());
	SpawnImpactEffectAtLocation(Hit, GetActorLocation());
	Destroy();
}

void AGrenadeLauncherProjectile::AddDamageAtLocation(const FHitResult& Hit, const FVector& ExplosiveLocation)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	TArray<AActor*> ignoreActors;
	ignoreActors.Init(this, 1);

	TArray<AActor*> outActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), ExplosiveLocation, AmmoData.DamageRadius, ObjectTypes, NULL, ignoreActors, outActors);

	//DrawDebugSphere(GetWorld(), GetActorLocation(), AmmoData.DamageRadius, 12, FColor::Red, true, 10.0f);

	for (size_t i = 0; i < outActors.Num(); i++)
	{
		AActor* HitActor = outActors[i];
		if (HitActor)
		{
			ITakeDamage* TakeDamageInterface = Cast<ITakeDamage>(HitActor);
			if (TakeDamageInterface)
			{
				TakeDamageInterface->TakeRadialDamage(AmmoData, CriticalHitModifier, Hit,ExplosiveLocation);
			}
		}
	}
}

void AGrenadeLauncherProjectile::SpawnImpactEffectAtLocation(const FHitResult& HitResult, const FVector& ExplosiveLocation)
{
	FTransform SpawnTransForm(FRotator(0, 0, 0), ExplosiveLocation);
	AGrenadeImpactEffect* ImpactEffect = Cast<AGrenadeImpactEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), GrenadeImpactEffectBP, SpawnTransForm));

	if (ImpactEffect != nullptr)
	{
		ImpactEffect->initializeParams(HitResult, HitResult.bBlockingHit, ExplosiveLocation);
		UGameplayStatics::FinishSpawningActor(ImpactEffect, SpawnTransForm);
	}
}

