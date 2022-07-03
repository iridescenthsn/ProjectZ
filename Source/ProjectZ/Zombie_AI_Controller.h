// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Zombie_AI_Controller.generated.h"

UENUM(BlueprintType)
enum class EAiState : uint8
{
	Patrolling,
	Chasing,
	Investigating,
};
/**
 * 
 */
UCLASS()
class PROJECTZ_API AZombie_AI_Controller : public AAIController
{
	GENERATED_BODY()

	AZombie_AI_Controller();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIPerception", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent* AIPerception;

	UFUNCTION()
	void OnAIPerceptionUpdated(const TArray<AActor*>&DetectedActors);

	UFUNCTION()
	void CheckIfReachedLocation();

	virtual void BeginPlay() override;

	UBlackboardComponent* BlackBoard;

	class AAI_Character* PawnRef;

	FTimerHandle Timer;
	
protected:

	UPROPERTY(EditAnywhere,Category="AiWalkSpeed")
	float ChaseSpeed;

	UPROPERTY(EditAnywhere,Category="AiWalkSpeed")
	float PatrollingSpeed;

	UPROPERTY(EditAnywhere,Category="AiWalkSpeed")
	float InvestigatingSpeed;

	UPROPERTY(EditDefaultsOnly)
	UBehaviorTree* TheTree;

	UPROPERTY(EditDefaultsOnly)
    TSubclassOf<class UAISense> Sight;
public:
	
	UFUNCTION(BlueprintCallable)
	void ChangeAiState(EAiState AiState);

	void SetPawnRef(AAI_Character* const PawnToSet) {PawnRef = PawnToSet;}

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FVector FirstGoalLocation;
	
};
