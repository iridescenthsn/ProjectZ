// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"	
#include "CoreMinimal.h"			
#include "TakeDamage.h"
#include "GameFramework/Character.h"	
#include "FPS_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFireWeaponDelegate,EWeaponType,WeaponType);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStopFireWeaponDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnWeaponSwitchDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractPressed);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnADSEnterDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnADSExitDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHudWeaponUpdateDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainHudUpdateDelegate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDeath);

class UCurveFloat;
class AWeaponBase;


UENUM(BlueprintType)
enum class EWeaponSlot : uint8
{
	FirstSlot,
	SecondSlot
};

UCLASS()
class PROJECTZ_API AFPS_Character : public ACharacter , public ITakeDamage
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPS_Character();

	UPROPERTY(BlueprintAssignable)
	FOnFireWeaponDelegate CharacterFireWeapon;

	UPROPERTY(BlueprintAssignable)
	FOnStopFireWeaponDelegate CharacterStopFireWeapon;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponSwitchDelegate CharacterWeaponSwitch;

	UPROPERTY(BlueprintAssignable)
	FOnInteractPressed InteractPressed;

	UPROPERTY(BlueprintAssignable)
	FOnADSEnterDelegate EventADSEnter;

	UPROPERTY(BlueprintAssignable)
	FOnADSExitDelegate EventADSExit;

	UPROPERTY(BlueprintAssignable)
	FOnHudWeaponUpdateDelegate UpdateWeaponHud;

	UPROPERTY(BlueprintAssignable)
	FOnMainHudUpdateDelegate UpdateMainHud;

	UPROPERTY(BlueprintAssignable)
	FOnPlayerDeath EventPlayerDeath;

	bool PickUpWeapon(TSubclassOf<AWeaponBase> WeaponToSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void Turn(float value);
	void LookUp(float value);
	virtual void Jump() override;

	void ADSEnter();
	void ADSExit();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh1P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Mesh3P;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class ACameraActor* DeathCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* DeathCameraTransform;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* PullDownCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* PullUpCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* EquipWeaponCurve;

	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat* ADSCurve;

	FTimeline ReloadPullDownTimeLine;

	FTimeline ReloadPullUpTimeline;

	FTimeline EquipWeaponTimeLine;

	FTimeline ADSTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float WeaponPullAlpha;

	UFUNCTION()
	void SetFOV(float value) const;
	
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

	UFUNCTION()
	void CheckIsNearWall();

	UPROPERTY(EditAnywhere, Category = "Property")
	float NearWallCheckRange=10;

	void SpawnSecondSlot(TSubclassOf<AWeaponBase> WeaponToSpawn);

	void SpawnFirstSlot(TSubclassOf<AWeaponBase> WeaponToSpawn);

	UPROPERTY(EditAnywhere,Category="Jumping")
	float JumpHeight=300;

	UPROPERTY(EditAnywhere, Category = "ADS")
	float DefaultFOV=90.0f;

	UPROPERTY(EditAnywhere, Category = "ADS")
	float CurrentFOV=90.0f;

	UPROPERTY(EditAnywhere, Category = "ADS")
	float TargetFOV=90.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	AWeaponBase* CurrentWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsReloading=false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bIsChangingWeapon=false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly , Category = "Weapon")
	bool bHasWeapon=false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bFirstSlotFull=false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bSecondSlotFull=false;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* WeaponSlot_01;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeaponBase* WeaponSlot_02;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponSlot WeaponSlot;

private:

	void EquipSlot1();
	void EquipSlot2();
	void EquipWeapon();
	void ShowWeapon() const;
	void Interact();
	
	void ReloadPullUp();

	UFUNCTION(BlueprintCallable)
	virtual void TakeMeleeDamage(float Damage) override;

	void RegenerateHealth();

	void PlayDeathRagdollAnimation();

	FTimerHandle HealthRegenHandle;

	//The time it takes for the camera to blend to the death camera
	UPROPERTY(EditAnywhere, Category = "Camera")
	float DeathCameraBlendTime=1.0f;

	//Rate of Regenerating health
	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthRegenRate;

	//The delay after taking damage to start regenerating health
	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthRegenDelay;

	//The amount of health to be added every time regenerate health is called
	UPROPERTY(EditAnywhere, Category = "Health")
	float HealthRegenAmount;
	
	UPROPERTY(EditAnywhere,Category="Health")
	float CurrentHealth=100.0f;
	
	UPROPERTY(EditAnywhere,Category="Health")
	float MaxHealth=100.0f;

	UPROPERTY(EditAnywhere,Category="Health")
	float CurrentArmor=100.0f;

	UPROPERTY(EditAnywhere,Category="Health")
	float MaxArmor=100.0f;

	UPROPERTY(VisibleAnywhere,Category="Health")
	bool bIsDead=false;
public:
	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	bool bCanFire = true;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ADS")
	bool IsADSing = false;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly, Category = "Property")
	bool bIsNearWall = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float MaxWalkSpeed=600.0f;

	UFUNCTION(BlueprintCallable)
	void OnFire();

	UFUNCTION(BlueprintCallable)
	void StopFire();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const {return CurrentHealth;}

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const {return MaxHealth;}

	UFUNCTION(BlueprintCallable)
	float GetCurrentArmor() const {return CurrentArmor;}

	UFUNCTION(BlueprintCallable)
	float GetMaxArmor() const {return MaxArmor;}

	UFUNCTION(BlueprintCallable)
	bool GetIsDead() const {return bIsDead;}
};
