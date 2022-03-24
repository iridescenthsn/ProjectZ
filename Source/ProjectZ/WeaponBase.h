// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"				
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

class AImpactEffect;

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	float BulletSpread=300;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	float CriticalHitModifier=1;

public:	
	//Name of the socket the gun will attach to
	UPROPERTY(EditAnywhere, Category = "Socket")
	FName SocketName;

	//The weapon type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WeaponType")
	EWeaponType WeaponType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Reload")
	float ReloadTime;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Recoil")
	float RecoilIntensity;

	virtual void WeaponFire();

	UFUNCTION()
	void Reload();

	bool HasReservedAmmo();

	FMagStatus MagStatus();
};
