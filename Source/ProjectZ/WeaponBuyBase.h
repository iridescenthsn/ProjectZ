// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"		
#include "GameFramework/Actor.h"
#include "WeaponBuyBase.generated.h"

UCLASS()
class PROJECTZ_API AWeaponBuyBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBuyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//The scene root of gun
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USceneComponent* SceneRoot;

	//Mesh of the weapon to buy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* GunMesh;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Mesh")
	class USphereComponent* Sphere;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Weapon")
	FName WeaponName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 WeaponCost;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AWeaponBase> Weapon;

	UPROPERTY()
	class AFPS_Character* Character;


	UFUNCTION()
	void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void PickUp();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
