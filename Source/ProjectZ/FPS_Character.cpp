// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBase.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FPS_Character.h"

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

}

// Called to bind functionality to input
void AFPS_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &AFPS_Character::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPS_Character::MoveRight);

	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &AFPS_Character::jump);
	PlayerInputComponent->BindAction("WeaponSlot1", IE_Pressed, this,&AFPS_Character::EquipSlot1);
	PlayerInputComponent->BindAction("WeaponSlot2", IE_Pressed, this,&AFPS_Character::EquipSlot2);
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

void AFPS_Character::jump()
{
	if (!(AFPS_Character::GetCharacterMovement()->IsFalling()))
	{
		LaunchCharacter(FVector(0, 0, JumpHeight), false, true);
	}
}

void AFPS_Character::EquipSlot1()
{
	SpawnWeapon(AssualtRifleBlueprint);
}

void AFPS_Character::EquipSlot2()
{
	SpawnWeapon(PistolBlueprint);
}


//Spawns and attaches weapon to the correct socket on mesh
void AFPS_Character::SpawnWeapon(TSubclassOf<AWeaponBase> WeaponToSpawn)
{
	WeaponSlot_01 = GetWorld()->SpawnActor<AWeaponBase>(WeaponToSpawn);
	WeaponSlot_01->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), WeaponSlot_01->SocketName);

	if (!bIsChangingWeapon)
	{
		bIsChangingWeapon = true;
		CurrentWeapon = WeaponSlot_01;
		bHasWeapon = true;
	}
}

