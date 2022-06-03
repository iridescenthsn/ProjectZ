// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Zombie_AI_Controller.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AZombie_AI_Controller : public AAIController
{
	GENERATED_BODY()

	AZombie_AI_Controller();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AIPerception", meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionComponent* AIPerception;

	UFUNCTION()
	void OnAIPerceptionUpdated(const TArray<AActor*>&DetectedActors);
};
