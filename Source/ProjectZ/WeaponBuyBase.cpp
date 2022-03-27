// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponBuyBase.h"
#include "FPS_Character.h"	
#include "GameFramework/RotatingMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "WeaponBase.h"


// Sets default values
AWeaponBuyBase::AWeaponBuyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a scene component and set it as seen root
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SceneRoot->SetupAttachment(RootComponent);

	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetupAttachment(SceneRoot);
	GunMesh->SetOnlyOwnerSee(false);
	GunMesh->CastShadow = false;
	GunMesh->bCastDynamicShadow = false;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	Sphere->SetupAttachment(SceneRoot);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
}

// Called when the game starts or when spawned
void AWeaponBuyBase::BeginPlay()
{
	Super::BeginPlay();

	Character = Cast<AFPS_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBuyBase::OnSphereBeginOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &AWeaponBuyBase::OnSphereEndOverlap);
}

void AWeaponBuyBase::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character->InteractPressed.AddUniqueDynamic(this, &AWeaponBuyBase::PickUp);
}

void AWeaponBuyBase::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Character->InteractPressed.Remove(this, TEXT("PickUp"));
}

void AWeaponBuyBase::PickUp()
{
	if (Character->PickUpWeapon(Weapon))
	{
		this->Destroy();
	}
}

// Called every frame
void AWeaponBuyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

