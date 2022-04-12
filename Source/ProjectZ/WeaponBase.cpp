// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "ImpactEffect.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "FPS_Character.h"
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
	SceneRoot->SetupAttachment(RootComponent);

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
	
	Player = Cast<AFPS_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	Camera = Player->FirstPersonCameraComponent;


	FOnTimelineFloat RecoilPitchTMFloat;
	RecoilPitchTMFloat.BindUFunction(this, FName(TEXT("AddRecoilPitch")));
	RecoilTimeLine.AddInterpFloat(RecoilPitchCurve, RecoilPitchTMFloat);


	FOnTimelineFloat RecoilYawTMFloat;
	RecoilYawTMFloat.BindUFunction(this, FName(TEXT("AddRecoilYaw")));
	RecoilTimeLine.AddInterpFloat(RecoilYawCurve, RecoilYawTMFloat);

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
	else if (RecoilTimelineDirection == ETimelineDirection::Backward)
	{
		RecoilTimeLine.TickTimeline(DeltaTime);
		RecoilTimeLine.TickTimeline(DeltaTime);
		RecoilTimeLine.TickTimeline(DeltaTime);
		RecoilTimeLine.TickTimeline(DeltaTime);
		RecoilTimeLine.TickTimeline(DeltaTime);
	}

	UE_LOG(LogTemp,Warning,TEXT("player pitch input %f"),PlayerPitchInput)

	UE_LOG(LogTemp, Warning, TEXT("player yaw input %f"), PlayerYawInput)
}

FHitResult AWeaponBase::CalculateShot()
{
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

	if (bHit)
	{
		AddDamage(HitResult);
	}

	return HitResult;
}

void AWeaponBase::AddDamage(FHitResult Hit)
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

void AWeaponBase::AddRecoil()
{
	if (RecoilYawCurve && RecoilPitchCurve)
	{
		RecoilAllAdedYaw = 0.0f;
		RecoilAllAdedPitch = 0.0f;
		AddRecoilTimesCalled = 0;

		PlayerPitchInput = 0.0f;
		PlayerYawInput = 0.0f;

		RecoilTimeLine.PlayFromStart();
		RecoilTimelineDirection = ETimelineDirection::Forward;
	}
}

void AWeaponBase::AddRecoilPitch(float value)
{
	AddRecoilTimesCalled++;

	if (RecoilTimelineDirection == ETimelineDirection::Forward)
	{
		RecoilAllAdedPitch += value;
		Player->AddControllerPitchInput(-value);
	}
	else
	{
		Player->AddControllerPitchInput(PitchPullDown);
	}
}

void AWeaponBase::AddRecoilYaw(float value)
{
	if (RecoilTimelineDirection==ETimelineDirection::Forward)
	{
		RecoilAllAdedYaw += value;
		Player->AddControllerYawInput(value);
	}
	else
	{
		Player->AddControllerYawInput(-YawPullDown);
	}
}

void AWeaponBase::RevertRecoil()
{
	RecoilAllAdedPitch = RecoilAllAdedPitch - PlayerPitchInput;
	RecoilAllAdedYaw = RecoilAllAdedYaw - PlayerYawInput;

	PitchPullDown = RecoilAllAdedPitch / AddRecoilTimesCalled;
	YawPullDown = RecoilAllAdedYaw / AddRecoilTimesCalled;

	RecoilTimeLine.Reverse();
	RecoilTimelineDirection = ETimelineDirection::Backward;
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
			SpawnDecal();
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

void AWeaponBase::SpawnDecal()
{
	FHitResult HitResult = CalculateShot();

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

	if (RecoilYawCurve&& RecoilPitchCurve)
	{
		StopRecoil();
		RevertRecoil();
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

