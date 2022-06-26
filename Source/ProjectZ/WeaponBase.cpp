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
#include "Perception/AISense_Hearing.h"


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
	
	//timeline shouldn't loop
	RecoilTimeLine.SetLooping(false);

	BulletSpread=HipFireBulletSpread;

	PerBulletRecoilPitch.SetNum(MaxAmmoInMag);
	PerBulletRecoilYaw.SetNum(MaxAmmoInMag);
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	RecoilTimeLine.TickTimeline(DeltaTime);
}

FHitResult AWeaponBase::CalculateShot() const
{
	//Randomize bullet rays
	FVector Startloc = Camera->GetComponentLocation();
	FVector Endloc = Startloc + (UKismetMathLibrary::GetForwardVector(Camera->GetComponentRotation()) * LineTraceRange);
	
	Endloc.X = Endloc.X + FMath::RandRange(-BulletSpread, BulletSpread);
	Endloc.Y = Endloc.Y + FMath::RandRange(-BulletSpread, BulletSpread);
	Endloc.Z = Endloc.Z + FMath::RandRange(-BulletSpread, BulletSpread);

	FHitResult HitResult;

	//Add player as ignored actor for line trace
	FCollisionQueryParams ActorToIgnore;
	ActorToIgnore.AddIgnoredActor(Player);
	ActorToIgnore.bReturnPhysicalMaterial = true;

	//Add Objects to trace by (static and dynamic)
	FCollisionObjectQueryParams ObjectsToTrace;
	ObjectsToTrace.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
	ObjectsToTrace.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);

	GetWorld()->LineTraceSingleByObjectType(HitResult,Startloc,Endloc,ObjectsToTrace,ActorToIgnore);

	//DrawDebugLine(GetWorld(), Startloc, Endloc, FColor::Green, false, 4);
	//DrawDebugBox(GetWorld(), HitResult.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, 4);

	return HitResult;
}


//Add damage to the hit actor if it has TakeDamage interface
void AWeaponBase::AddDamage(const FHitResult &Hit) const
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


void AWeaponBase::AmmoShellEject() const
{
	if (AmmoShellClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GunMesh->GetSocketRotation(FName(TEXT("AmmoEject")));
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
	RecoilTimeLine.PlayFromStart();
}

//Adds recoil pitch when timeline is going forward and revert recoil when going backwards
void AWeaponBase::AddRecoilPitch(float Value)
{
	const float PitchToAdd= FMath::Lerp(PerBulletRecoilPitch[NumberOfShot],0.0f,Value);
	Player->AddControllerPitchInput(-PitchToAdd);

	RecoilAllAddedPitch-=PitchToAdd;
}


//Add yaw recoil when timeline is going forward
void AWeaponBase::AddRecoilYaw(float Value)
{
	const float YawToAdd=FMath::Lerp(PerBulletRecoilYaw[NumberOfShot],0.0f,Value);
	Player->AddControllerYawInput(YawToAdd);

	RecoilAllAddedYaw+=YawToAdd;
}


//Calculate Revert recoil based on the amount of the recoil added and player pitch input
void AWeaponBase::RevertRecoil()
{
	Player->AddControllerPitchInput(-PitchPullDown);
	Player->AddControllerYawInput(-YawPullDown);

	TimesCalled++;

	if (TimesCalled==100)
	{
		GetWorldTimerManager().ClearTimer(RecoilReverseHandle);
		TimesCalled=0;
	}
}

void AWeaponBase::ResetRecoil()
{
	if (!bIsWeaponFiring)
	{
		NumberOfShot--;
		NumberOfShot=FMath::Clamp(NumberOfShot,0,MaxAmmoInMag-1);
		if (NumberOfShot==0)
		{
			GetWorldTimerManager().ClearTimer(RecoilResetHandle);
			UE_LOG(LogTemp,Warning,TEXT("reset"))
		}
	}
}

void AWeaponBase::CalculateReverseRecoil()
{
	/*
	If same directions then ignore player input

	If opposite directions

		If player input abs is less than recoil abs then add them together

		If player input abs is more than or equal to recoil abs then dont pull 
	*/

	if (FMath::Sign(PlayerPitchInput)!=FMath::Sign(RecoilAllAddedPitch))
	{
		UE_LOG(LogTemp,Warning,TEXT("pitch pull down : %f Yaw pull down : %f"),RecoilAllAddedPitch,RecoilAllAddedYaw)
		if (FMath::Abs(PlayerPitchInput)<FMath::Abs(RecoilAllAddedPitch))
		{
			RecoilAllAddedPitch += PlayerPitchInput;
		}
		else
		{
			RecoilAllAddedPitch = 0;
		}
	}
	PitchPullDown=RecoilAllAddedPitch/100;

	if (FMath::Sign(PlayerYawInput) != FMath::Sign(RecoilAllAddedYaw))
	{
		if (FMath::Abs(PlayerYawInput) < FMath::Abs(RecoilAllAddedYaw))
		{
			RecoilAllAddedYaw += PlayerYawInput;
		}
		else
		{
			RecoilAllAddedYaw = 0;
		}
	}
	YawPullDown=RecoilAllAddedYaw/100;
}

//Firing method on weapon reduces 1 ammo everytime its called
void AWeaponBase::WeaponFire()
{
	if (Player->bCanFire && bIsReadyToFire)
	{
		if (MagStatus().bHasAmmo)
		{
			bIsWeaponFiring = true;
			
			CurrentAmmoInMag--;

			Shoot();
			UAISense_Hearing::ReportNoiseEvent(GetWorld(),GetActorLocation(),1,Player,0,FName("AI_Noise"));
			AddRecoil();

			NumberOfShot++;
			NumberOfShot=FMath::Clamp(NumberOfShot,0,MaxAmmoInMag-1);

			//Stops gun recoil animation and sets the weapon shooting readiness state
			if (!bIsWeaponAuto)
			{
				Player->CharacterFireWeapon.Broadcast(WeaponType);
				
				bIsReadyToFire = false;
				GetWorldTimerManager().SetTimer(ShootingDelayHandle, this, &AWeaponBase::SetWeaponState, FireRate, false);

				GetWorldTimerManager().SetTimer(StopFiringHandle, this, &AWeaponBase::CharacterStopFireWeapon, StopFireRate, false);
			}

			GunMesh->PlayAnimation(FireAnimation, false);

			if (CameraShakeBP)
			{
				const auto CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
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
	bIsReadyToFire=true;
	GetWorldTimerManager().ClearTimer(ShootingDelayHandle);
}

void AWeaponBase::CharacterStopFireWeapon()
{
	Player->CharacterStopFireWeapon.Broadcast();
	GetWorldTimerManager().ClearTimer(StopFiringHandle);
}

void AWeaponBase::SpawnImpactEffect(const FHitResult &HitResult) const
{
	const FTransform SpawnTransForm(FRotator(0, 0, 0), HitResult.ImpactPoint);
	AImpactEffect* ImpactEffect = Cast<AImpactEffect>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(), ImpactEffectBP, SpawnTransForm));

	if (ImpactEffect != nullptr)
	{
		ImpactEffect->initialize(HitResult, HitResult.bBlockingHit);
		UGameplayStatics::FinishSpawningActor(ImpactEffect, SpawnTransForm);
	}
}

void AWeaponBase::StopFire()
{
	if (bIsWeaponFiring)
	{
		Player->CharacterStopFireWeapon.Broadcast();

		CalculateReverseRecoil();

		GetWorldTimerManager().SetTimer(RecoilReverseHandle, this, &AWeaponBase::RevertRecoil, RecoilReverseSpeed, true);
		
		GetWorldTimerManager().SetTimer(RecoilResetHandle, this, &AWeaponBase::ResetRecoil, ResetRecoilRate, true);

		bIsWeaponFiring = false;
	}
}

//Reload function that fills the mag
void AWeaponBase::Reload()
{
	//fill the mag full or fill with any ammo that's left
	const int32 ToFill= MaxAmmoInMag-CurrentAmmoInMag;
	
	if (CurrentReservedAmmo>=ToFill)
	{
		CurrentAmmoInMag=MaxAmmoInMag;
		CurrentReservedAmmo-=ToFill;
	}
	else
	{
		CurrentAmmoInMag+=CurrentReservedAmmo;
		CurrentReservedAmmo=0;
	}
}

bool AWeaponBase::HasReservedAmmo() const
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

FMagStatus AWeaponBase::MagStatus() const
{
	FMagStatus Mag;
	CurrentAmmoInMag > 0 ? Mag.bHasAmmo = true : Mag.bHasAmmo = false;

	CurrentAmmoInMag == MaxAmmoInMag ? Mag.bMagFull = true : Mag.bMagFull = false;

	return Mag;
}


