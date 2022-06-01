// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "FindRandoLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API UFindRandoLocation : public UBTTaskNode
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:

	UPROPERTY(EditAnywhere,Category="BlackBoard")
	struct FBlackboardKeySelector Location;
};
