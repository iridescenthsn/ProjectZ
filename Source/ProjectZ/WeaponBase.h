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


	//Gun mesh 1st person view
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USkeletalMeshComponent* GunMesh;

	//The scene root of gun
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	class USceneComponent* SceneRoot;

	//The weapon type
	UPROPERTY(BlueprintReadOnly, Category = "WeaponType")
	EWeaponType WeaponType;

	//Name of the socket the gun will attach to
	UPROPERTY(EditAnywhere,Category="Socket")
	FName SocketName;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
