// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_Character.h"
#include "Blueprint/UserWidget.h"
#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraActor.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPS_Character::AFPS_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;

	//Create 3rd person mesh (used for death ragdoll)
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh3P"));
	Mesh3P->SetOnlyOwnerSee(false);
	Mesh3P->SetupAttachment(GetCapsuleComponent());
	Mesh3P->bCastDynamicShadow = true;
	Mesh3P->CastShadow = true;
	Mesh3P->SetHiddenInGame(true);
	Mesh3P->SetSimulatePhysics(false);

	DeathCameraTransform= CreateDefaultSubobject<UArrowComponent>(TEXT("Death Camera Transform"));
	DeathCameraTransform->SetupAttachment(GetCapsuleComponent());

	MaxWalkSpeed=GetCharacterMovement()->MaxWalkSpeed;
}

// Called when the game starts or when spawned
void AFPS_Character::BeginPlay()
{
	Super::BeginPlay();

	DefaultArmsTransform = Mesh1P->GetRelativeTransform();

	if (ADSCurve)
	{
		FOnTimelineFloat ADSTimelineFloat;
		ADSTimelineFloat.BindUFunction(this, FName(TEXT("SetFOV")));
		ADSTimeline.AddInterpFloat(ADSCurve, ADSTimelineFloat);
	}

	if (EquipWeaponCurve)
	{
		FOnTimelineEvent TimeLineFinished;
		TimeLineFinished.BindUFunction(this, FName("EquipWeaponFinished"));
		EquipWeaponTimeLine.SetTimelineFinishedFunc(TimeLineFinished);

		FOnTimelineEvent WeaponSwitchTime;
		WeaponSwitchTime.BindUFunction(this, FName("WeaponSwitch"));
		EquipWeaponTimeLine.AddEvent(0.25, WeaponSwitchTime);

		FOnTimelineFloat TimeLineProgress;
		TimeLineProgress.BindUFunction(this, FName("SetAlpha"));
		EquipWeaponTimeLine.AddInterpFloat(EquipWeaponCurve, TimeLineProgress);
		EquipWeaponTimeLine.SetLooping(false);
	}
	
	if (PullDownCurve)
	{
		FOnTimelineEvent PullDownFinished;
		PullDownFinished.BindUFunction(this, FName("OnReloadPullDownFinished"));
		ReloadPullDownTimeLine.SetTimelineFinishedFunc(PullDownFinished);
		
		FOnTimelineFloat PullDownProgress;
		PullDownProgress.BindUFunction(this, FName("SetAlpha")); 
		ReloadPullDownTimeLine.AddInterpFloat(PullDownCurve, PullDownProgress);
		ReloadPullDownTimeLine.SetLooping(false);
	}
	
	if (PullUpCurve)
	{
		FOnTimelineEvent PullUpFinished;
		PullUpFinished.BindUFunction(this, FName("OnReloadPullUpFinished"));
		ReloadPullUpTimeline.SetTimelineFinishedFunc(PullUpFinished);
	 
		FOnTimelineFloat PullUpProgress;
		PullUpProgress.BindUFunction(this, FName("SetAlpha"));
		ReloadPullUpTimeline.AddInterpFloat(PullUpCurve, PullUpProgress);
		ReloadPullUpTimeline.SetLooping(false);
	}

	UGameplayStatics::GetPlayerController(GetWorld(),0)->SetViewTarget(this);

	UpdateWeaponHud.Broadcast();
}

// Called every frame
void AFPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ReloadPullDownTimeLine.TickTimeline(DeltaTime);
	ReloadPullUpTimeline.TickTimeline(DeltaTime);
	EquipWeaponTimeLine.TickTimeline(DeltaTime);

	ADSTimeline.TickTimeline(DeltaTime);

	WeaponSway(DeltaTime);
}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &AFPS_Character::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPS_Character::LookUp);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);

	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &AFPS_Character::Jump);
	PlayerInputComponent->BindAction("WeaponSlot1", IE_Pressed, this,&AFPS_Character::EquipSlot1);
	PlayerInputComponent->BindAction("WeaponSlot2", IE_Pressed, this,&AFPS_Character::EquipSlot2);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPS_Character::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPS_Character::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPS_Character::Reload);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPS_Character::Interact);
	PlayerInputComponent->BindAction("ADS", IE_Pressed, this, &AFPS_Character::ADSEnter);
	PlayerInputComponent->BindAction("ADS", IE_Released, this, &AFPS_Character::ADSExit);
}

//Move forward and backwards
void AFPS_Character::MoveForward(float value)
{
	if ( value!=0 )
	{
		AddMovementInput(GetActorForwardVector(), value);
		CheckIsNearWall();
	}
}

//Move right and left
void AFPS_Character::MoveRight(float value)
{
	if (value != 0)
	{
		AddMovementInput(GetActorRightVector(), value);
		CheckIsNearWall();
	}
}

void AFPS_Character::Turn(float value)
{
	if (value!=0)
	{
		AddControllerYawInput(value);
		if (CurrentWeapon)
		{
			CurrentWeapon->PlayerYawInput += value;
		}

		CheckIsNearWall();
	}
}

void AFPS_Character::LookUp(float value)
{
	if (value!=0)
	{
		AddControllerPitchInput(value);
		if (CurrentWeapon)
		{
			CurrentWeapon->PlayerPitchInput += value;
		}

		CheckIsNearWall();
	}
}

void AFPS_Character::Jump()
{
	if (!(GetCharacterMovement()->IsFalling()))
	{
		LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
	}
}

void AFPS_Character::ADSEnter()
{
	if (IsADSing==false)
	{
		if (CurrentWeapon)
		{
			GetCharacterMovement()->MaxWalkSpeed=CurrentWeapon->GetAdsWalkSpeed();

			CurrentWeapon->SetBulletSpread(CurrentWeapon->GetAdsBulletSpread());
			
			IsADSing = true;
			ADSTimeline.Play();
			EventADSEnter.Broadcast();

			if (CurrentWeapon->GetWeaponType() == EWeaponType::SniperRifle)
			{
				CurrentWeapon->GetGunMesh()->SetHiddenInGame(true);
				Mesh1P->SetHiddenInGame(true);
			}
		}
	}
}

void AFPS_Character::ADSExit()
{
	if (IsADSing==true)
	{
		if (CurrentWeapon)
		{
			GetCharacterMovement()->MaxWalkSpeed=MaxWalkSpeed;
			CurrentWeapon->SetBulletSpread(CurrentWeapon->GetHipFireBulletSpread());
			
			IsADSing = false;
			ADSTimeline.Reverse();
			EventADSExit.Broadcast();

			if (CurrentWeapon->GetWeaponType() == EWeaponType::SniperRifle)
			{
				CurrentWeapon->GetGunMesh()->SetHiddenInGame(false);
				Mesh1P->SetHiddenInGame(false);
			}
		}
	}
}

void AFPS_Character::SetFOV(float value) const
{
	const float FOV = FMath::Lerp(DefaultFOV, CurrentWeapon->GetAdsFov(), value);
	const FTransform& Transform = UKismetMathLibrary::TLerp(IsADSing? Mesh1P->GetRelativeTransform() : DefaultArmsTransform ,
		CalculateAdsTransform(),value);

	Mesh1P->SetRelativeTransform(Transform);
	FirstPersonCameraComponent->SetFieldOfView(FOV);
}

void AFPS_Character::EquipSlot1()
{
	if (bFirstSlotFull)
	{
		if (CurrentWeapon!=WeaponSlot_01)
		{
			if (IsADSing)
			{
				ADSExit();
			}

			EquipWeapon();
		}
	}
}

void AFPS_Character::EquipSlot2()
{
	if (bSecondSlotFull)
	{
		if (CurrentWeapon!=WeaponSlot_02)
		{
			if (IsADSing)
			{
				ADSExit();
			}
			
			EquipWeapon();
		}
	}
}

void AFPS_Character::EquipWeapon()
{
	if (!bIsChangingWeapon)
	{
		if (bIsReloading)
		{
			bIsReloading = false;
		}
		
		GetWorldTimerManager().ClearTimer(CurrentWeapon->AutoFireHandle);
		GetWorldTimerManager().ClearTimer(ReloadTimerHandle);
		
		ReloadPullUpTimeline.Stop();
		ReloadPullUpTimeline.SetPlaybackPosition(0.0f,true);

		ReloadPullDownTimeLine.Stop();
		ReloadPullDownTimeLine.SetPlaybackPosition(0.0f,true);
		
		bIsChangingWeapon = true; 
		bCanFire = false;
		
		EquipWeaponTimeLine.PlayFromStart();
	}
}

void AFPS_Character::ShowWeapon() const
{
	if (CurrentWeapon==WeaponSlot_01)
	{
		WeaponSlot_01->SetActorHiddenInGame(false);

		if (WeaponSlot_02)
		{
			WeaponSlot_02->SetActorHiddenInGame(true);
		}
	}
	if (CurrentWeapon==WeaponSlot_02)
	{
		WeaponSlot_02->SetActorHiddenInGame(false);

		if (WeaponSlot_01)
		{
			WeaponSlot_01->SetActorHiddenInGame(true);
		}
	}
}

//Spawns and attaches weapon to the correct socket on mesh
bool AFPS_Character::PickUpWeapon(TSubclassOf<AWeaponBase> WeaponToSpawn)
{
	bool bSuccessful;

	switch (WeaponSlot)
	{

	case EWeaponSlot::FirstSlot:

		if (!bFirstSlotFull)
		{
			SpawnFirstSlot(WeaponToSpawn);
			bSuccessful = true;

			UpdateWeaponHud.Broadcast();
			return bSuccessful;
		}
		else
		{
			if (!bSecondSlotFull)
			{
				WeaponSlot = EWeaponSlot::SecondSlot;
				PickUpWeapon(WeaponToSpawn);
				return bSuccessful = true;
			}
			else
			{
				bSuccessful = false;
				return bSuccessful;
			}
		}

		break;

	case EWeaponSlot::SecondSlot:

		if (!bSecondSlotFull)
		{
			if (bIsReloading)
			{
				bIsReloading=false;
			}
			SpawnSecondSlot(WeaponToSpawn);
			bSuccessful = true;

			UpdateWeaponHud.Broadcast();
			return bSuccessful;
		} 
		else
		{
			bSuccessful = false;
			return bSuccessful;
		}

		break;

	default:

		return false;
		break;

	}
}

void AFPS_Character::SpawnSecondSlot(TSubclassOf<AWeaponBase> WeaponToSpawn)
{
	WeaponSlot_02 = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn);
	WeaponSlot_02->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		true), WeaponSlot_02->GetSocketName());
	
	bSecondSlotFull = true;
	bHasWeapon = true;
	WeaponSlot = EWeaponSlot::SecondSlot;
	CurrentWeapon = WeaponSlot_02;
	ShowWeapon();
	CharacterWeaponSwitch.Broadcast();
}

void AFPS_Character::SpawnFirstSlot(TSubclassOf<AWeaponBase> WeaponToSpawn)
{
	WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn);
	WeaponSlot_01->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
		true), WeaponSlot_01->GetSocketName());
	bFirstSlotFull = true;
	bHasWeapon = true;
	WeaponSlot = EWeaponSlot::FirstSlot;
	CurrentWeapon = WeaponSlot_01;
	ShowWeapon();
	CharacterWeaponSwitch.Broadcast();
}

void AFPS_Character::WeaponSwitch()
{
	if (WeaponSlot==EWeaponSlot::FirstSlot)
	{
		WeaponSlot=EWeaponSlot::SecondSlot;
		CurrentWeapon=WeaponSlot_02;
	}
	else
	{
		WeaponSlot=EWeaponSlot::FirstSlot;
		CurrentWeapon=WeaponSlot_01;
	}

	UpdateWeaponHud.Broadcast();
	
	ShowWeapon();
	bHasWeapon = true;
	CurrentWeapon->SetIsReadyToFire(true);
	CharacterWeaponSwitch.Broadcast();
}

void AFPS_Character::EquipWeaponFinished()
{
	bIsChangingWeapon = false;
	bCanFire = true;
}

//Check if player is near a wall or object
void AFPS_Character::CheckIsNearWall()
{
	if (!CurrentWeapon)
	{
		return;
	}

	FVector Startloc;
	FVector Endloc;

	Startloc = Mesh1P->GetSocketLocation(FName(TEXT("NearWallCheck_Socket")));
	Endloc = Startloc + (UKismetMathLibrary::GetForwardVector(Mesh1P->GetSocketRotation
		(FName(TEXT("NearWallCheck_Socket")))) * NearWallCheckRange);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(CurrentWeapon);
	CollisionParams.AddIgnoredActor(this);

	FCollisionObjectQueryParams TraceObjectParams;
	TraceObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FHitResult Hit;

	bIsNearWall = GetWorld()->LineTraceSingleByObjectType(Hit, Startloc, Endloc,
		TraceObjectParams,CollisionParams);

	/*DrawDebugLine(GetWorld(), Startloc, Endloc, FColor::Green, false, 0.2);
	DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Cyan, false, 0.2);*/
}

void AFPS_Character::OnFire()
{
	if (bCanFire&&bHasWeapon&& !bIsNearWall)
	{
		if (CurrentWeapon->MagStatus().bHasAmmo)
		{
			CurrentWeapon->PlayerPitchInput=0.0f;
			CurrentWeapon->PlayerYawInput=0.0f;
			CurrentWeapon->SetRecoilAllAddedPitch(0.0f);
			CurrentWeapon->SetRecoilAllAddedYaw(0.0f);
			
			CurrentWeapon->WeaponFire();
		}
		else if (CurrentWeapon->HasReservedAmmo())
		{
			Reload();
		}
	}
}

void AFPS_Character::StopFire()
{
	if (CurrentWeapon&& CurrentWeapon->IsWeaponAuto())
	{
		CurrentWeapon->StopFire();
	}
}

void AFPS_Character::Reload()
{
	if (bHasWeapon)
	{
		if (!(bIsChangingWeapon || bIsReloading))
		{
			if (CurrentWeapon->HasReservedAmmo())
			{
				if (!CurrentWeapon->MagStatus().bMagFull)
				{
					if (IsADSing)
					{
						ADSExit();
					}
					
					GetWorldTimerManager().ClearTimer(CurrentWeapon->AutoFireHandle);

					CurrentWeapon->SetIsWeaponFiring(false);
					bIsReloading = true;
					bCanFire = false;

					UpdateWeaponHud.Broadcast();

					ReloadPullDownTimeLine.PlayFromStart();
				}
			}
		}
	}
}

void AFPS_Character::Interact()
{
	InteractPressed.Broadcast();
}

FTransform AFPS_Character::CalculateAdsTransform() const
{
	const FTransform& Transform =UKismetMathLibrary::MakeRelativeTransform(Mesh1P->GetComponentTransform(),
		CurrentWeapon->GetGunMesh()->GetSocketTransform(FName("ADS_Socket")));
	return Transform;
}

void AFPS_Character::SetAlpha(float value)
{
	WeaponPullAlpha = value;
}

void AFPS_Character::OnReloadPullDownFinished()
{
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &AFPS_Character::ReloadPullUp,
		CurrentWeapon->GetReloadTime(), false);
}

void AFPS_Character::OnReloadPullUpFinished()
{
	bIsReloading = false;
	bCanFire = true;
	CurrentWeapon->SetIsReadyToFire(true);
	CurrentWeapon->Reload();

	UpdateWeaponHud.Broadcast();
}

void AFPS_Character::ReloadPullUp()
{
	ReloadPullUpTimeline.PlayFromStart();
}

//Called when taking melee damage
void AFPS_Character::TakeMeleeDamage(float Damage)
{
	if (!bIsDead)
	{
		//If we have enough armor use it to block damage. else block as much
		//as possible with armor and block the rest with health
		if (CurrentArmor>=Damage)
		{
			CurrentArmor-=Damage;
		}
		else
		{
			Damage-=CurrentArmor;
			CurrentArmor=0.0f;
			CurrentHealth-=Damage;

			//call regenerate health function on a loop with a timer
			GetWorldTimerManager().SetTimer(HealthRegenHandle,this,&AFPS_Character::RegenerateHealth,HealthRegenRate,true,HealthRegenDelay);

			FMath::Clamp(CurrentHealth,0.0f,MaxHealth);

			if (CurrentHealth<=0.0f)
			{
				//if player is dead ragdoll the 3rd person mesh
				bIsDead=true;
				EventPlayerDeath.Broadcast();

				PlayDeathRagdollAnimation();
				
				GetWorldTimerManager().ClearTimer(HealthRegenHandle);
			}
		}
	}
}


//Regenerates health up to max health
void AFPS_Character::RegenerateHealth()
{
	CurrentHealth+=HealthRegenAmount;
	FMath::Clamp(CurrentHealth,0.0f,MaxHealth);

	if (CurrentHealth>=MaxHealth)
	{
		GetWorldTimerManager().ClearTimer(HealthRegenHandle);
	}
}


//Hides first person mesh and shows the 3rd person mesh and simulates ragdoll
//Also blends out to a different camera so we can see the ragdoll body
void AFPS_Character::PlayDeathRagdollAnimation()
{
	Mesh1P->SetHiddenInGame(true);
	Mesh3P->SetHiddenInGame(false);
	Mesh3P->SetSimulatePhysics(true);

	const auto& OldController = UGameplayStatics::GetPlayerController(GetWorld(),0);

	DeathCamera= GetWorld()->SpawnActor<ACameraActor>(DeathCameraTransform->GetComponentLocation(),DeathCameraTransform->GetComponentRotation());
	
	DeathCamera->GetCameraComponent()->SetConstraintAspectRatio(false);

	UGameplayStatics::GetPlayerController(GetWorld(),0)->UnPossess();
	
	if (CurrentWeapon)
	{
		const FAttachmentTransformRules TransformRules= FAttachmentTransformRules::SnapToTargetNotIncludingScale;
		CurrentWeapon->AttachToComponent(Mesh3P,TransformRules,TEXT("WeaponPoint"));
	}

	const FAttachmentTransformRules TransformRules = FAttachmentTransformRules::KeepWorldTransform;
	DeathCamera->AttachToComponent(Mesh3P,TransformRules);
	
	OldController->SetViewTargetWithBlend(DeathCamera,DeathCameraBlendTime);

}

void AFPS_Character::WeaponSway(float DeltaTime) const
{
	if (!CurrentWeapon)
	{
		return;
	}
	if (IsADSing)
	{
		//Calculate fina; rotation based on input
		FRotator FinalRot;
		FinalRot.Yaw=GetInputAxisValue(FName("Turn"))*MaxSwayDegree;
		FinalRot.Roll=GetInputAxisValue(FName("LookUp"))*MaxSwayDegree;
		FinalRot.Pitch=GetInputAxisValue(FName("Turn"))*MaxSwayDegree;

		//interp to the final rotation
		FRotator DeltaRot=FMath::RInterpTo(CurrentWeapon->GetGunMesh()->GetRelativeRotation(),FinalRot,DeltaTime,SwaySpeed);

		//clamp it to max degrees
		DeltaRot.Yaw=FMath::Clamp(DeltaRot.Yaw,-MaxSwayDegree,MaxSwayDegree);
		DeltaRot.Pitch=FMath::Clamp(DeltaRot.Pitch,-MaxSwayDegree,MaxSwayDegree);
		DeltaRot.Roll=FMath::Clamp(DeltaRot.Roll,-MaxSwayDegree,MaxSwayDegree);

		//Set the relative rotation
		CurrentWeapon->GetGunMesh()->SetRelativeRotation(DeltaRot);
	}
	else
	{
		//Calculate fina; rotation based on input
		FRotator FinalRot;
		FinalRot.Yaw=GetInputAxisValue(FName("Turn"))*MaxSwayDegree;
		FinalRot.Roll=GetInputAxisValue(FName("LookUp"))*MaxSwayDegree;
		FinalRot.Pitch=GetInputAxisValue(FName("Turn"))*MaxSwayDegree;

		//interp to the final rotation
		FRotator DeltaRot=FMath::RInterpTo(Mesh1P->GetRelativeRotation(),FinalRot,DeltaTime,SwaySpeed);

		//clamp it to max degrees
		DeltaRot.Yaw=FMath::Clamp(DeltaRot.Yaw,-MaxSwayDegree,MaxSwayDegree);
		DeltaRot.Pitch=FMath::Clamp(DeltaRot.Pitch,-MaxSwayDegree,MaxSwayDegree);
		DeltaRot.Roll=FMath::Clamp(DeltaRot.Roll,-MaxSwayDegree,MaxSwayDegree);
		DeltaRot.Yaw-=90.0f;

		//Set the relative rotation
		Mesh1P->SetRelativeRotation(DeltaRot);
	}
}

