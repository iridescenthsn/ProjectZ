// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "ImpactEffect.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPS_Character.h"
#include "PistolAmmoShell.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TakeDamage.h"


// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a scene component and set it as seen root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	// Create gun mesh and attach it to scene root
	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetOnlyOwnerSee(false);
	GunMesh->CastShadow = false;
	GunMesh->bCastDynamicShadow = false;
	GunMesh->SetupAttachment(SceneRoot);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	//Get camera and player ref
	Player = Cast<AFPS_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Camera = Player->FirstPersonCameraComponent;

	//Timeline float for recoil pitch binding
	if (RecoilPitchCurve)
	{
		FOnTimelineFloat RecoilPitchTMFloat;
		RecoilPitchTMFloat.BindUFunction(this, FName(TEXT("AddRecoilPitch")));
		RecoilTimeLine.AddInterpFloat(RecoilPitchCurve, RecoilPitchTMFloat);

		if (!bIsWeaponAuto)
		{
			FOnTimelineEvent RecoilTimeLineFinished;
			RecoilTimeLineFinished.BindUFunction(this, FName(TEXT("StopFire")));
			RecoilTimeLine.SetTimelineFinishedFunc(RecoilTimeLineFinished);
		}
	}
	
	//Timeline float for recoil Yaw binding
	if (RecoilYawCurve)
	{
		FOnTimelineFloat RecoilYawTMFloat;
		RecoilYawTMFloat.BindUFunction(this, FName(TEXT("AddRecoilYaw")));
		RecoilTimeLine.AddInterpFloat(RecoilYawCurve, RecoilYawTMFloat);
	}
	
	//timeline shouldnt loop
	RecoilTimeLine.SetLooping(false);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RecoilTimelineDirection==ETimelineDirection::Forward)
	{
		RecoilTimeLine.TickTimeline(DeltaTime);
	}
	
	//Advance the timeline faster when reverting recoil
	if (RecoilTimelineDirection==ETimelineDirection::Backward)
	{
		for (size_t i = 0; i < ReverseTimeLineSpeed; i++)
		{
			RecoilTimeLine.TickTimeline(DeltaTime);
		}
	}
}

FHitResult AWeaponBase::CalculateShot()
{
	//Randomize bullet rays
	FVector startloc = Camera->GetComponentLocation();
	FVector endloc = startloc + (UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * LineTraceRange);
	endloc.X = endloc.X + FMath::RandRange(-BulletSpread, BulletSpread);
	endloc.Y = endloc.Y + FMath::RandRange(-BulletSpread, BulletSpread);
	endloc.Z = endloc.Z + FMath::RandRange(-BulletSpread, BulletSpread);

	FHitResult HitResult;

	//Add player as ignored actor for line trace
	FCollisionQueryParams ActorToIgnore;
	ActorToIgnore.AddIgnoredActor(Player);
	ActorToIgnore.bReturnPhysicalMaterial = true;

	//Add Objects to trace by (static and dynamic)
	FCollisionObjectQueryParams ObjectsToTrace;
	ObjectsToTrace.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectsToTrace.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult,startloc,endloc,ObjectsToTrace,ActorToIgnore);

	//DrawDebugLine(GetWorld(), startloc, endloc, FColor::Green, false, 4);
	//DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, 4);

	return HitResult;
}


//Add damage to the hit actor if it has takedamage interface
void AWeaponBase::AddDamage(const FHitResult &Hit)
{
	AActor* HitActor = Hit.GetActor();
	if (HitActor)
	{
		ITakeDamage* TakeDamageInterface = Cast<ITakeDamage>(HitActor);
		if (TakeDamageInterface)
		{
			TakeDamageInterface->TakeDamage(AmmoData, CriticalHitModifier, Hit);
		}
	}
}


void AWeaponBase::AmmoShellEject()
{
	if (AmmoShellClass != NULL)
	{
		UWorld* const World = GetWorld();
		if (World != NULL)
		{
			FRotator SpawnRotation = GunMesh->GetSocketRotation(FName(TEXT("AmmoEject")));
			//SpawnRotation.Yaw = FMath::RandRange(-360,360);
			const FVector SpawnLocation = GunMesh->GetSocketLocation(FName(TEXT("AmmoEject")));

			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


			World->SpawnActor<APistolAmmoShell>(AmmoShellClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
		}
	}
}

//Plays the recoil timeline
void AWeaponBase::AddRecoil()
{
	if (RecoilYawCurve && RecoilPitchCurve)
	{
		StartPlayingTimeLine();
	}
	else if (RecoilPitchCurve && !bIsWeaponAuto)
	{
		StartPlayingTimeLine();
	}
}


void AWeaponBase::StartPlayingTimeLine()
{
	RecoilTimelineDirection = ETimelineDirection::Forward;
	RecoilTimeLine.PlayFromStart();
}

//Adds recoil pitch when timeline is going forward and revert recoil when going backwards
void AWeaponBase::AddRecoilPitch(float value)
{
	if (RecoilTimelineDirection == ETimelineDirection::Forward)
	{
		Player->AddControllerPitchInput(-value);
	}
	else
	{
		Player->AddControllerPitchInput(value);
	}
}


//Add yaw recoil when timeline is going forward
void AWeaponBase::AddRecoilYaw(float value)
{
	if (RecoilTimelineDirection==ETimelineDirection::Forward)
	{
		Player->AddControllerYawInput(value);
	}
}


//Calculate Revert recoil based on the amount of the recoil added and player pitch input
void AWeaponBase::RevertRecoil()
{
	RecoilTimelineDirection = ETimelineDirection::Backward;
	RecoilTimeLine.ReverseFromEnd();
}

void AWeaponBase::StopRecoil()
{
	RecoilTimeLine.Stop();
}

//Firing method on weapon reduces 1 ammo everytime its called
void AWeaponBase::WeaponFire()
{
	if (Player->bCanFire)
	{
		if (MagStatus().bHasAmmo)
		{
			bIsWeaponFiring = true;

			Player->CharacterFireWeapon.Broadcast(WeaponType);

			CurrentAmmoInMag--;

			Shoot();

			//Stops gun recoil animation and sets the weapon shooting readiness state
			if (!bIsWeaponAuto)
			{
				Player->bCanFire = false;
				GetWorldTimerManager().SetTimer(ShootingDelayHandle, this, &AWeaponBase::SetWeaponState, DelayBetweenShots, false);

				GetWorldTimerManager().SetTimer(StopFiringHandle, this, &AWeaponBase::CharacterStopFireWeapon, StopFireRate, false);
			}

			GunMesh->PlayAnimation(FireAnimation, false);

			if (CameraShakeBP)
			{
				auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
				CameraManager->PlayCameraShake(CameraShakeBP);
			}

			AmmoShellEject();
		}
		else
		{
			StopFire();
			Player->bCanFire = false;

			if (HasReservedAmmo())
			{
				Player->Reload();
			}
		}
	}
	
}

void AWeaponBase::Shoot()
{

}

void AWeaponBase::SetWeaponState()
{
	Player->bCanFire = true;
	GetWorldTimerManager().ClearTimer(ShootingDelayHandle);
}

void AWeaponBase::CharacterStopFireWeapon()
{
	Player->CharacterStopFireWeapon.Broadcast();
	GetWorldTimerManager().ClearTimer(StopFiringHandle);
}

void AWeaponBase::SpawnImpactEffect(const FHitResult &HitResult)
{
	FTransform SpawnTransForm(FRotator(0, 0, 0), HitResult.ImpactPoint);
	AImpactEffect* ImpactEffect = Cast<AImpactEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ImpactEffectBP, SpawnTransForm));

	if (ImpactEffect != nullptr)
	{
		ImpactEffect->initialize(HitResult, HitResult.bBlockingHit);
		UGameplayStatics::FinishSpawningActor(ImpactEffect, SpawnTransForm);
	}
}

void AWeaponBase::StopFire()
{
	bIsWeaponFiring = false;
	Player->CharacterStopFireWeapon.Broadcast();

	if (RecoilTimelineDirection==ETimelineDirection::Forward)
	{
		if (RecoilYawCurve && RecoilPitchCurve)
		{
			StopRecoil();
			RevertRecoil();
		}
		else if (RecoilPitchCurve && !bIsWeaponAuto)
		{
			StopRecoil();
			RevertRecoil();
		}
	}
}

//Reload function that fills the mag
void AWeaponBase::Reload()
{
	//fill the mag full or fill with any ammo thats left
	CurrentAmmoInMag = FMath::Min(MaxAmmoInMag, CurrentReservedAmmo);
	CurrentReservedAmmo -= CurrentAmmoInMag;
}

bool AWeaponBase::HasReservedAmmo()
{
	if (CurrentReservedAmmo > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

FMagStatus AWeaponBase::MagStatus()
{
	FMagStatus Mag;
	CurrentAmmoInMag > 0 ? Mag.bHasAmmo = true : Mag.bHasAmmo = false;

	CurrentAmmoInMag == MaxAmmoInMag ? Mag.bMagFull = true : Mag.bMagFull = false;

	return Mag;
}


