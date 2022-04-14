// Fill out your copyright notice in the Description page of Project Settings.

#include "FPS_Character.h"
#include "Blueprint/UserWidget.h"
#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"


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
}

// Called when the game starts or when spawned
void AFPS_Character::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPS_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ReloadCurveTimeLine.TickTimeline(DeltaTime);
	EquipWeaponTimeLine.TickTimeline(DeltaTime);
}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &AFPS_Character::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AFPS_Character::LookUp);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);

	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &AFPS_Character::jump);
	PlayerInputComponent->BindAction("WeaponSlot1", IE_Pressed, this,&AFPS_Character::EquipSlot1);
	PlayerInputComponent->BindAction("WeaponSlot2", IE_Pressed, this,&AFPS_Character::EquipSlot2);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AFPS_Character::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AFPS_Character::StopFire);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AFPS_Character::Reload);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AFPS_Character::Interact);
}

//Move forward and backwards
void AFPS_Character::MoveForward(float value)
{
	if ( value!=0 )
	{
		AddMovementInput(GetActorForwardVector(), value);
	}
}

//Move right and left
void AFPS_Character::MoveRight(float value)
{
	if (value != 0)
	{
		AddMovementInput(GetActorRightVector(), value);
	}
}

void AFPS_Character::Turn(float value)
{
	AddControllerYawInput(value);
}

void AFPS_Character::LookUp(float value)
{
	AddControllerPitchInput(value);
	if (CurrentWeapon)
	{
		CurrentWeapon->PlayerPitchInput += value;
	}
}

void AFPS_Character::jump()
{
	if (!(AFPS_Character::GetCharacterMovement()->IsFalling()))
	{
		LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
	}
}

void AFPS_Character::EquipSlot1()
{
	if (bFirstSlotFull)
	{
		if (CurrentWeapon!=WeaponSlot_01)
		{
			CurrentWeapon = WeaponSlot_01;
			WeaponSlot = EWeaponSlot::FirstSlot;
			EquipWeapon(WeaponSlot_01);
		}
	}
}

void AFPS_Character::EquipSlot2()
{
	if (bSecondSlotFull)
	{
		if (CurrentWeapon!=WeaponSlot_02)
		{
			CurrentWeapon = WeaponSlot_02;
			WeaponSlot = EWeaponSlot::SecondSlot;
			EquipWeapon(WeaponSlot_02);
		}
	}
}

void AFPS_Character::EquipWeapon(AWeaponBase* WeaponToEquip)
{
	if (!bIsChangingWeapon)
	{
		if (bIsReloading)
		{
			bIsReloading = false;
		}

		bIsChangingWeapon = true;
		bCanFire = false;

		EquipWeaponTimelineFunc();
	}
}

void AFPS_Character::ShowWeapon(AWeaponBase* WeaponToEquip)
{
	if (CurrentWeapon==WeaponSlot_01)
	{
		WeaponSlot_01->SetActorHiddenInGame(false);

		if (WeaponSlot_02!=nullptr)
		{
			WeaponSlot_02->SetActorHiddenInGame(true);
		}
	}
	if (CurrentWeapon==WeaponSlot_02)
	{
		WeaponSlot_02->SetActorHiddenInGame(false);

		if (WeaponSlot_01!=nullptr)
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
			SpawnSecondSlot(WeaponToSpawn);
			bSuccessful = true;
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
	WeaponSlot_02->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSlot_02->SocketName);
	bSecondSlotFull = true;
	bHasWeapon = true;
	WeaponSlot = EWeaponSlot::SecondSlot;
	CurrentWeapon = WeaponSlot_02;
	ShowWeapon(CurrentWeapon);
	CharacterWeaponSwitch.Broadcast();
}

void AFPS_Character::SpawnFirstSlot(TSubclassOf<AWeaponBase> WeaponToSpawn)
{
	WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn);
	WeaponSlot_01->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSlot_01->SocketName);
	bFirstSlotFull = true;
	bHasWeapon = true;
	WeaponSlot = EWeaponSlot::FirstSlot;
	CurrentWeapon = WeaponSlot_01;
	ShowWeapon(CurrentWeapon);
	CharacterWeaponSwitch.Broadcast();
}

void AFPS_Character::EquipWeaponTimelineFunc()
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
	EquipWeaponTimeLine.PlayFromStart();
}

void AFPS_Character::WeaponSwitch()
{
	ShowWeapon(CurrentWeapon);
	bHasWeapon = true;
	CharacterWeaponSwitch.Broadcast();
}

void AFPS_Character::EquipWeaponFinished()
{
	bIsChangingWeapon = false;
	bCanFire = true;
}

void AFPS_Character::OnFire()
{
	if (bCanFire&&bHasWeapon)
	{
		if (CurrentWeapon->MagStatus().bHasAmmo)
		{
			CurrentWeapon->WeaponFire();
			CurrentWeapon->AddRecoil();
		}
		else if (CurrentWeapon->HasReservedAmmo())
		{
			Reload();
		}
	}
}

void AFPS_Character::StopFire()
{
	if (CurrentWeapon)
	{
		if (CurrentWeapon->bIsWeaponFiring)
		{
			CurrentWeapon->StopFire();
		}
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
					bIsReloading = true;
					bCanFire = false;

					ReloadPullDown();
				}
			}
		}
	}
}

void AFPS_Character::Interact()
{
	InteractPressed.Broadcast();
}

void AFPS_Character::ReloadPullDown()
{
	if (PullDownCurve)
	{
		FOnTimelineEvent TimeLineFinished;
		TimeLineFinished.BindUFunction(this, FName("OnReloadPullDownFinished"));
		ReloadCurveTimeLine.SetTimelineFinishedFunc(TimeLineFinished);
		
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("SetAlpha"));
		ReloadCurveTimeLine.AddInterpFloat(PullDownCurve, TimelineProgress);
		ReloadCurveTimeLine.SetLooping(false);
		ReloadCurveTimeLine.PlayFromStart();
	}
}

void AFPS_Character::SetAlpha(float value)
{
	WeaponPullAlpha = value;
}

void AFPS_Character::OnReloadPullDownFinished()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AFPS_Character::ReloadPullUp, CurrentWeapon->ReloadTime, false);
}

void AFPS_Character::OnReloadPullUpFinished()
{
	bIsReloading = false;
	bCanFire = true;
	CurrentWeapon->Reload();
}

void AFPS_Character::ReloadPullUp()
{
	if (PullUpCurve)
	{
		FOnTimelineEvent TimeLineFinished;
		TimeLineFinished.BindUFunction(this, FName("OnReloadPullUpFinished"));
		ReloadCurveTimeLine.SetTimelineFinishedFunc(TimeLineFinished);
	
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("SetAlpha"));
		ReloadCurveTimeLine.AddInterpFloat(PullUpCurve, TimelineProgress);
		ReloadCurveTimeLine.SetLooping(false);
		ReloadCurveTimeLine.PlayFromStart();
	}
}

