// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"				
#include "Components/TimelineComponent.h"		
#include "GameFramework/Actor.h"	
#include "WeaponBase.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Pistol,
	AssaultRifle,
	ShotGun,
	SniperRifle,
	GrenadeLauncher,
	RocketLauncher
};

USTRUCT(BlueprintType)
struct FMagStatus
{
	GENERATED_USTRUCT_BODY()
public:

	bool bHasAmmo;
	bool bMagFull;
};

USTRUCT(BlueprintType)
struct FAmmoData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float CriticalHitChance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	float DamageRadius;
};


class AImpactEffect;
class UCurveFloat;
class APistolAmmoShell;

UCLASS()
class PROJECTZ_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Calculates bullet spread and line traces with it
	FHitResult CalculateShot() const;

	//Apply damage to actors which can take Damage
	void AddDamage(const FHitResult &Hit) const;

	//Ejects ammo shell after firing
	void AmmoShellEject() const;

	//Ammo shell to Spawn
	UPROPERTY(EditDefaultsOnly, Category = "Ammoshell")
	TSubclassOf <APistolAmmoShell> AmmoShellClass;

	//Player ref
	UPROPERTY(EditDefaultsOnly, Category = "PlayerRef")
	class AFPS_Character* Player;

	//Player Camera for line tracing
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* Camera;

	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	TSubclassOf<class UCameraShake> CameraShakeBP;

	//Gun mesh 1st person view
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMesh;

	//The scene root of gun
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USceneComponent* SceneRoot;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	class UAnimationAsset* FireAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	UCurveFloat* RecoilPitchCurve;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	UCurveFloat* RecoilYawCurve;
	
	FTimeline RecoilTimeLine;

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> RecoilTimelineDirection;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf <AImpactEffect> ImpactEffectBP;

	UPROPERTY(EditDefaultsOnly, Category = "LineTrace")
	float LineTraceRange=10000;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 CurrentAmmoInMag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 MaxAmmoInMag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 CurrentReservedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	int32 MaxReservedAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float CriticalHitModifier=1;


public:	
	//Name of the socket the gun will attach to
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName SocketName;

	//The weapon type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
	EWeaponType WeaponType;

	//Properties of the ammo of the gun such as damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	FAmmoData AmmoData;

	FTimerHandle StopFiringHandle;

	FTimerHandle ShootingDelayHandle;

	FTimerHandle RecoilResetHandle;
	
	FTimerHandle RecoilReverseHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Recoil")
	float StopFireRate=0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Property")
	float ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float DelayBetweenShots=0.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float AnimRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	int32 ReverseTimeLineSpeed=1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	bool bIsWeaponAuto;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float ADSFov=85.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float ADSWalkSpeed=400.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float HipFireBulletSpread=300;

	UPROPERTY(VisibleAnywhere,Category="Property")
	float BulletSpread;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float ADSBulletSpread=0;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	float ZOffset;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	float YOffset;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	float XOffset;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	float XRot;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	float YRot;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Animation")
	float ZRot;
	
	//function called when fire input pressed
	virtual void WeaponFire();

	//Shoots based on the weapon type
	virtual void Shoot();

	void SetWeaponState();

	void CharacterStopFireWeapon();

	void SpawnImpactEffect(const FHitResult &HitResult) const;

	UFUNCTION()
	virtual void StopFire();

	UFUNCTION()
	void AddRecoil();

	virtual void StartPlayingTimeLine();

	UFUNCTION()
	virtual void AddRecoilPitch(float Value);

	UFUNCTION()
	virtual void AddRecoilYaw(float Value);

	UFUNCTION()
	virtual void RevertRecoil();

	void ResetRecoil();

	void CalculateReverseRecoil();

	UFUNCTION()
	void StopRecoil();

	UFUNCTION()
	void Reload();

	bool HasReservedAmmo() const;

	bool bIsWeaponFiring;

	FMagStatus MagStatus() const;

	float RecoilAllAddedPitch = 0.0f;
	float RecoilAllAddedYaw = 0.0f;

	float PitchPullDown = 0.0f;
	float YawPullDown = 0.0f;

	float PlayerPitchInput=0.0f;
	float PlayerYawInput = 0.0f;
	
	int32 NumberOfShot=0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float ResetRecoilRate=0.2;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Recoil")
	TArray<float> PerBulletRecoilPitch;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Recoil")
	TArray<float> PerBulletRecoilYaw;

	int32 TimesCalled=0;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Recoil")
	float RecoilReverseSpeed=0.001f;

public:

	USkeletalMeshComponent* GetGunMesh() const { return GunMesh; }

};
