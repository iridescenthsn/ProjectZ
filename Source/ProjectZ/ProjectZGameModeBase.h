// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectZGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AProjectZGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

	UPROPERTY()
	TArray<AActor*> Spawners;

	void SpawnZombies();

	void NextRound();

protected:
	
	UPROPERTY(EditDefaultsOnly,Category="Spawner Class")
	TSubclassOf<AActor> SpawnerClass;

	UPROPERTY(VisibleAnywhere,Category="AI")
	int32 ZombiesAlive=0;
	
	//The number of zombies that will spawn in this current round
	UPROPERTY(VisibleAnywhere,Category="AI")
	int32 NumberOfZombiesToSpawn=0;

	//Zombies Spawned this round so far
	UPROPERTY(VisibleAnywhere,Category="AI")
	int32 ZombiesSpawnedThisRound=0;

	UPROPERTY(EditAnywhere,Category="AI")
	float SpawningRate=2.0f;

	UPROPERTY(VisibleAnywhere,Category="Round")
	int32 CurrentRound=0;
	
	const int32 MaxAliveAtTime=30;

	UPROPERTY(EditAnywhere,Category="Round")
	float TimeBetweenRounds=5.0f;

	FTimerHandle SpawnerHandle;
public:
	
	int32 GetZombiesAlive() const {return ZombiesAlive;}

	void SetZombiesAlive(const int32 Alive) {ZombiesAlive = Alive;}

	void IsRoundOver();
};
