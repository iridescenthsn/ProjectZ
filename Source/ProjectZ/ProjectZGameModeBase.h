// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectZGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FUpdateRoundInfo,int,Round,int,ZombiesAlive);

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

	UPROPERTY(VisibleAnywhere,Category="Stats")
	int32 ZombiesKilled=0;

	UPROPERTY(VisibleAnywhere,Category="Stats")
	int32 TotalScore=0;

	FTimerHandle SpawnerHandle;
public:
	
	int32 GetZombiesAlive() const {return ZombiesAlive;}

	void SetZombiesAlive(const int32 Alive) {ZombiesAlive = Alive;}

	void IsRoundOver();

	UPROPERTY(BlueprintAssignable)
	FUpdateRoundInfo UpdateRoundUI;

	UFUNCTION(BlueprintCallable)
	int32 GetCurrentRound() const {return CurrentRound;}

	UFUNCTION(BlueprintCallable)
	int32 GetZombiesKilled() const {return ZombiesKilled;}

	void SetZombiesKilled(const int32 Value) {ZombiesKilled = Value;}

	UFUNCTION(BlueprintCallable)
	int32 GetTotalScore() const {return TotalScore;}

	void SetTotalScore(const int32 Value) {TotalScore = Value;}
};
