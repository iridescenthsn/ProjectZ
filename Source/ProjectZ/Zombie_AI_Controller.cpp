// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AI_Controller.h"

#include "AI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"

AZombie_AI_Controller::AZombie_AI_Controller()
{
	AIPerception=CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AZombie_AI_Controller::OnAIPerceptionUpdated);
}

void AZombie_AI_Controller::OnAIPerceptionUpdated(const TArray<AActor*>& DetectedActors)
{
	for (const auto& Actor:DetectedActors)
	{
		FActorPerceptionBlueprintInfo Info;
		AIPerception->GetActorsPerception(Actor,Info);
		
		GetBlackboardComponent()->SetValueAsBool((FName("Can see player")),Info.LastSensedStimuli[0].WasSuccessfullySensed());
		if (Info.LastSensedStimuli[0].WasSuccessfullySensed())
		{
			Cast<AAI_Character>(GetPawn())->GetCharacterMovement()->MaxWalkSpeed=500.0f;
			UE_LOG(LogTemp,Warning,TEXT("Max walk speed 500"))
		}
		else
		{
			Cast<AAI_Character>(GetPawn())->GetCharacterMovement()->MaxWalkSpeed=75.0f;
			UE_LOG(LogTemp,Warning,TEXT("Max walk speed 500"))
		}
	}
}


