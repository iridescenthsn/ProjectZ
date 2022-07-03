// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI_Character.h"
#include "GameFramework/Actor.h"
#include "ZombieSpawner.generated.h"

class UArrowComponent;

UCLASS()
class PROJECTZ_API AZombieSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AZombieSpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly,Category="AI class")
	TSubclassOf<AAI_Character> AIClass;

	UPROPERTY(EditDefaultsOnly,Category="AI class")
	class UBehaviorTree* AIBehaviorTree;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* SpawnPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawn", meta = (AllowPrivateAccess = "true"))
	UArrowComponent* FirstGoal;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category="Property")
	bool bIsActive=true;
public:

	UFUNCTION(BlueprintCallable)
	bool GetIsActive() const{return bIsActive;}

	UFUNCTION(BlueprintCallable)
	void SetIsActive(const bool Value) {bIsActive = Value;}

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnZombies() const;
};
