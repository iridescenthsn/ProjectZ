// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetAIOwner()->MoveToLocation(OwnerComp.GetBlackboardComponent()->GetValueAsVector(FName("Target Location")));

	return  EBTNodeResult::Succeeded;
}
