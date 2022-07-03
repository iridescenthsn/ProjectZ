// Copyright Epic Games, Inc. All Rights Reserved.


#include "ProjectZGameModeBase.h"
#include "ZombieSpawner.h"
#include "Kismet/GameplayStatics.h"

void AProjectZGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(),SpawnerClass,Spawners);

	if (Spawners.Num()>0)
	{
		NextRound();
	}
}

void AProjectZGameModeBase::SpawnZombies()
{
	if (NumberOfZombiesToSpawn>ZombiesSpawnedThisRound&& MaxAliveAtTime>=ZombiesAlive)
	{
		int32 RandomSpawnerIndex;
		while (true)
		{
			RandomSpawnerIndex = FMath::RandRange(0,Spawners.Num()-1);
			if (Cast<AZombieSpawner>(Spawners[RandomSpawnerIndex])->GetIsActive())
			{
				break;
			}
		}
		
		Cast<AZombieSpawner>(Spawners[RandomSpawnerIndex])->SpawnZombies();
		ZombiesSpawnedThisRound++;
		ZombiesAlive++;
		UpdateRoundUI.Broadcast(CurrentRound,ZombiesAlive);
		
		GetWorldTimerManager().SetTimer(SpawnerHandle,this,&AProjectZGameModeBase::SpawnZombies,SpawningRate,true);
	}
}

void AProjectZGameModeBase::NextRound()
{
	CurrentRound++;
	NumberOfZombiesToSpawn= CurrentRound*2+9;
	ZombiesSpawnedThisRound=0;
	UpdateRoundUI.Broadcast(CurrentRound,ZombiesAlive);

	FTimerHandle StartRoundHandle; 
	GetWorldTimerManager().SetTimer(StartRoundHandle,this,&AProjectZGameModeBase::SpawnZombies,TimeBetweenRounds);
}

void AProjectZGameModeBase::IsRoundOver()
{
	if (ZombiesSpawnedThisRound>=NumberOfZombiesToSpawn&&ZombiesAlive<=0)
	{
		GetWorldTimerManager().ClearTimer(SpawnerHandle);
		NextRound();
	}
}
