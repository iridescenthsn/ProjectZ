// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie_AI_Controller.h"
#include "AI_Character.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"

AZombie_AI_Controller::AZombie_AI_Controller()
{
	AIPerception=CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComp"));

	AIPerception->OnPerceptionUpdated.AddDynamic(this, &AZombie_AI_Controller::OnAIPerceptionUpdated);
}

void AZombie_AI_Controller::OnAIPerceptionUpdated(const TArray<AActor*>& DetectedActors)
{
	if (!BlackBoard)
	{
		return;
	}
	
	for (const auto& Actor:DetectedActors)
	{
		if (!Actor->ActorHasTag(FName("Player")))
		{
			return;
		}
		
		//reset this value every time
		BlackBoard->SetValueAsBool(FName("TookDamage"),false);
		
		//Get perception of each detected actor
		FActorPerceptionBlueprintInfo Info;
		AIPerception->GetActorsPerception(Actor,Info);

		//Check if we seen player or lost sight
		const bool WasSuccessfullySeen = Info.LastSensedStimuli[0].WasSuccessfullySensed();
		BlackBoard->SetValueAsBool((FName("Can see player")),WasSuccessfullySeen);
		UE_LOG(LogTemp,Warning,TEXT("Was seen : %s"),WasSuccessfullySeen ? TEXT("True") : TEXT("False"))

		//Set the player key and chase if we see player
		//Otherwise Investigate
		if (WasSuccessfullySeen)
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
		
		//Investigate around Own location
		const bool WasSuccessfullyDamaged=Info.LastSensedStimuli[2].WasSuccessfullySensed();
		if (WasSuccessfullyDamaged)
		{
			ChangeAiState(EAiState::Patrolling);
			ChangeAiState(EAiState::Investigating);
			BlackBoard->SetValueAsBool(FName("TookDamage"),true);
		}

		//Investigate sound location
		const bool WasSuccessFullyHeard = Info.LastSensedStimuli[1].WasSuccessfullySensed();
		const FName& Tag = Info.LastSensedStimuli[1].Tag;
		if (WasSuccessFullyHeard && Tag.IsEqual(FName("AI_Noise")))
		{
			ChangeAiState(EAiState::Patrolling);
			ChangeAiState(EAiState::Investigating);
			
			BlackBoard->SetValueAsBool(FName("TookDamage"),false);
			BlackBoard->SetValueAsVector(FName("Target Location"),Info.LastSensedStimuli[1].StimulusLocation);
		}
	}
}

void AZombie_AI_Controller::CheckIfReachedLocation()
{ 
	if((GetPawn()->GetActorLocation() - FirstGoalLocation).Size()<=100)
	{
		RunBehaviorTree(TheTree); 
		BlackBoard=GetBlackboardComponent();

		FTimerHandle Handle;
		GetWorldTimerManager().SetTimer(Handle,[&]
		{
			AIPerception->SetSenseEnabled(Sight,true);
		},1,false);
		
		GetWorldTimerManager().ClearTimer(Timer);
	}
}


void AZombie_AI_Controller::BeginPlay()
{
	Super::BeginPlay();

	if (!PawnRef)
	{
		PawnRef=Cast<AAI_Character>(GetPawn());
	}
	
	FTimerHandle Handle;
	GetWorldTimerManager().SetTimer(Handle,[&]
	{
		MoveToLocation(FirstGoalLocation);
	},0.1,false);
	
	GetWorldTimerManager().SetTimer(Timer,this,&AZombie_AI_Controller::CheckIfReachedLocation,0.1,true);
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


