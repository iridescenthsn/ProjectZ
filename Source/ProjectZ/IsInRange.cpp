// Fill out your copyright notice in the Description page of Project Settings.


#include "IsInRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"

UIsInRange::UIsInRange(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bNotifyBecomeRelevant = true;
}

void UIsInRange::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const float DistanceToPlayer= OwnerComp.GetAIOwner()->GetPawn()->GetDistanceTo(UGameplayStatics::GetPlayerPawn(GetWorld(),0));

	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsInRange"),DistanceToPlayer<=MeleeRange);
}
