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
	GENERATED_USTRUCT_BODY()
public:

	float Damage;
	float CriticalHitChance;
};


class AImpactEffect;
class UCurveFloat;

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
	FHitResult CalculateShot();

	//Apply damage to actors which can take Damage
	void AddDamage(FHitResult Hit);

	//Player ref
	UPROPERTY(EditDefaultsOnly, Category = "PlayerRef")
	class AFPS_Character* Player;

	//Player Camera for line tracing
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	class UCameraComponent* Camera;

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

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	FTimeline RecoilTimeLine;

	UPROPERTY()
	TEnumAsByte<ETimelineDirection::Type> RecoilTimelineDirection;

	//Recoil Timeline reverse play rate
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	float RecoilTimelineReversePR=2.0f;

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
	float BulletSpread=300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float CriticalHitModifier=1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float AutomaticFireRate = 0.1;

public:	
	//Name of the socket the gun will attach to
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName SocketName;

	//The weapon type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
	EWeaponType WeaponType;

	//Properties of the ammo of the gun such as damage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	FAmmoData AmmoData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Property")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float AnimRecoil;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Recoil")
	bool bTimeLineisBound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	bool bIsWeaponAuto;

	virtual void WeaponFire();

	void SpawnDecal();

	virtual void StopFire();

	UFUNCTION()
	void AddRecoil();

	UFUNCTION()
	void AddRecoilPitch(float value);

	UFUNCTION()
	void AddRecoilYaw(float value);

	UFUNCTION()
	void RevertRecoil();

	UFUNCTION()
	void StopRecoil();

	UFUNCTION()
	void Reload();

	bool HasReservedAmmo();

	bool bIsWeaponFiring;

	FMagStatus MagStatus();

	float RecoilAllAdedYaw = 0.0f;

	float RecoilAllAdedPitch = 0.0f;

	float PitchPullDown = 0.0f;

	float YawPullDown = 0.0f;

	float PlayerYawInput = 0.0f;

	float PlayerPitchInput=0.0f;
	
	int32 AddRecoilTimesCalled = 0;
};
