// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* BlackBoard = OwnerComp.GetBlackboardComponent();
	UObject* PlayerObject =BlackBoard->GetValueAsObject(FName("Player"));
	OwnerComp.GetAIOwner()->MoveToActor(Cast<AActor>(PlayerObject));
	return  EBTNodeResult::Succeeded;
}
