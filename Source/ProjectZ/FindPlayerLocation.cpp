// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerComp.GetBlackboardComponent()->SetValueAsVector(FName(TEXT("Target Location")),
		UGameplayStatics::GetPlayerPawn(GetWorld(),0)->GetActorLocation());
	
	return EBTNodeResult::Succeeded;
}
