// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieSpawner.h"

#include "Zombie_AI_Controller.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Components/ArrowComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AZombieSpawner::AZombieSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnPoint=CreateDefaultSubobject<UArrowComponent>(FName("Spawn Point"));
	FirstGoal = CreateDefaultSubobject<UArrowComponent>(FName("First Goal"));
}

// Called when the game starts or when spawned
void AZombieSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AZombieSpawner::SpawnZombies() const
{
	//begin spawning actor
	AAI_Character* AICharacter = Cast<AAI_Character>(UGameplayStatics::BeginDeferredActorSpawnFromClass(GetWorld(),
		AIClass,GetActorTransform(),ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn));

	//if spawned set the pawn refrence and then finish spawning
	if (AICharacter)
	{
		AICharacter->SpawnDefaultController();
		Cast<AZombie_AI_Controller>(AICharacter->GetController())->SetPawnRef(AICharacter);
		UGameplayStatics::FinishSpawningActor(AICharacter,GetActorTransform());

		Cast<AZombie_AI_Controller>(AICharacter->GetController())->FirstGoalLocation=FirstGoal->GetComponentLocation();
	}
}

// Called every frame
void AZombieSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

