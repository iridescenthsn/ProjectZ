// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"	
#include "GameFramework/Character.h"
#include "FPS_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireWeaponDelegate,EWeaponType,WeaponType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopFireWeaponDelegate);

class UCurveFloat;
class AWeaponBase;

UCLASS()
class PROJECTZ_API AFPS_Character : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPS_Character();

	UPROPERTY(BlueprintAssignable)
	FOnFireWeaponDelegate CharacterFireWeapon;

	UPROPERTY(BlueprintAssignable)
	FOnStopFireWeaponDelegate CharacterStopFireWeapon;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void jump();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* PullDownCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* PullUpCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* EquipWeaponCurve;

	FTimeline ReloadCurveTimeLine;

	FTimeline EquipWeaponTimeLine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WeaponPullAlpha;

	UFUNCTION()
	void SetAlpha(float value);

	UFUNCTION()
	void OnReloadPullDownFinished();

	UFUNCTION()
	void OnReloadPullUpFinished();

	UFUNCTION()
	void WeaponSwitch();

	UFUNCTION()
	void EquipWeaponFinished();

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
	void OnFire();
	void StopFire();
	void Reload();

	void ReloadPullDown();
	void ReloadPullUp();
	void EquipWeaponTimelineFunc();

	void SpawnWeapon(TSubclassOf<AWeaponBase> WeaponToSpawn);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

};
