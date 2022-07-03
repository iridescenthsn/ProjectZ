// Fill out your copyright notice in the Description page of Project Settings.


#include "AutomaticDoor.h"
#include "ZombieSpawner.h"

AAutomaticDoor::AAutomaticDoor()
{
	LeftDoor= CreateDefaultSubobject<UStaticMeshComponent>(FName("LeftDoor"));
	LeftDoor->SetupAttachment(SceneRoot);
	LeftDoor->Mobility=TEnumAsByte<EComponentMobility::Type>(EComponentMobility::Movable);
	LeftDoor->SetCanEverAffectNavigation(false);

	RightDoor= CreateDefaultSubobject<UStaticMeshComponent>(FName("RightDoor"));
	RightDoor->SetupAttachment(SceneRoot);
	RightDoor->Mobility=TEnumAsByte<EComponentMobility::Type>(EComponentMobility::Movable);
	RightDoor->SetCanEverAffectNavigation(false);
}

void AAutomaticDoor::OnUnlock()
{
	GetWorldTimerManager().SetTimer(Handle,[&]
	{
		LeftDoor->AddLocalOffset(FVector(1.0f,0.0f,0.0f));
		RightDoor->AddLocalOffset(FVector(1.0f,0.0f,0.0f));
		AllAddedOffset+=1.0f;
		if (AllAddedOffset>=200.0f)
		{
			if (TheSpawner)
			{
				TheSpawner->SetIsActive(true);
			}
			
			GetWorldTimerManager().ClearTimer(Handle);
			Destroy();
		}
	},0.01,true);
}
