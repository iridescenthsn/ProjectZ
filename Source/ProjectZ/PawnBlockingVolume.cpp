// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnBlockingVolume.h"

#include "AI_Character.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"

// Sets default values
APawnBlockingVolume::APawnBlockingVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Blocker=CreateDefaultSubobject<UBoxComponent>(FName("Blocker"));

	Blocker->OnComponentEndOverlap.AddDynamic(this, &APawnBlockingVolume::BlockZombie);
}

// Called when the game starts or when spawned
void APawnBlockingVolume::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APawnBlockingVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APawnBlockingVolume::BlockZombie(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) 
{
	const ECollisionChannel CollisionChannel= ECollisionChannel::ECC_GameTraceChannel3;
	const AAI_Character* Zombie =Cast<AAI_Character>(OtherActor);
	if (Zombie)
	{
		Zombie->GetCapsuleComponent()->SetCollisionResponseToChannel(CollisionChannel,ECollisionResponse::ECR_Block);
	}
}

