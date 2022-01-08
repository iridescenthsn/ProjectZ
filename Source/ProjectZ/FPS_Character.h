// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"	
#include "GameFramework/Character.h"
#include "FPS_Character.generated.h"

class AWeaponBase;

UCLASS()
class PROJECTZ_API AFPS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPS_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void jump();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(EditAnywhere,Category="Jumping")
	float JumpHeight=300;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bCanFire=true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsReloading=false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsChangingWeapon=false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Weapon")
	bool bHasWeapon=false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* WeaponSlot_01;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* WeaponSlot_02;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf <AWeaponBase> AssualtRifleBlueprint;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf <AWeaponBase> PistolBlueprint;
private:

	void EquipSlot1();
	void EquipSlot2();

	void SpawnWeapon(TSubclassOf<AWeaponBase> WeaponToSpawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
