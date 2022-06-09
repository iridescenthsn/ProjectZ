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
		const bool bWasSuccessfullySensed = Info.LastSensedStimuli[0].WasSuccessfullySensed();
		
		BlackBoard->SetValueAsBool((FName("Can see player")),bWasSuccessfullySensed);
		
		if (bWasSuccessfullySensed)
		{
			ChangeAiState(EAiState::Chasing);
			BlackBoard->SetValueAsObject(FName("Player"),Actor);
		}
		else
		{
			ChangeAiState(EAiState::Investigating);
			BlackBoard->ClearValue(FName("Player"));
			BlackBoard->SetValueAsVector(FName("Target Location"),Info.LastSensedStimuli[0].StimulusLocation);
		}
	}
}


void AZombie_AI_Controller::BeginPlay()
{
	Super::BeginPlay();

	PawnRef=Cast<AAI_Character>(GetPawn());

	BlackBoard= GetBlackboardComponent();
}

void AZombie_AI_Controller::ChangeAiState(EAiState AiState)
{
	BlackBoard->SetValueAsEnum(FName("AIState"),static_cast<uint8>(AiState));

	switch (AiState)
	{
	case EAiState::Chasing:
		PawnRef->GetCharacterMovement()->MaxWalkSpeed=ChaseSpeed;
		break;

	case EAiState::Investigating:
		PawnRef->GetCharacterMovement()->MaxWalkSpeed=InvestigatingSpeed;
		break;

	case EAiState::Patrolling:
		PawnRef->GetCharacterMovement()->MaxWalkSpeed=PatrollingSpeed;
		break;
	}
}


