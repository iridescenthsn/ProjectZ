// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "IsInRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UIsInRange : public UBTService
{
	GENERATED_BODY()

	UIsInRange(const FObjectInitializer& ObjectInitializer);

	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere)
	float MeleeRange=100.0f;
};
