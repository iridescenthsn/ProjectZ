// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandoLocation.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UFindRandoLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const FVector AILocation = OwnerComp.GetAIOwner()->GetPawn()->GetActorLocation();

	UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	FNavLocation RandomLocation;
	NavSys->GetRandomReachablePointInRadius(AILocation,1000,RandomLocation);
	
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("RandomLocation")),RandomLocation.Location);

	return EBTNodeResult::Succeeded;
	
}
