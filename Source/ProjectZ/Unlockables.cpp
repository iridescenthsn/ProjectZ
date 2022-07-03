// Fill out your copyright notice in the Description page of Project Settings.


#include "Unlockables.h"
#include "FPS_Character.h"
#include "NavModifierComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AUnlockables::AUnlockables()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot= CreateDefaultSubobject<USceneComponent>(FName("SceneRoot"));
	SetRootComponent(SceneRoot);
	SceneRoot->SetHiddenInGame(true);
	SceneRoot->SetCanEverAffectNavigation(false);
	
	NavModifier= CreateDefaultSubobject<UNavModifierComponent>(FName("Nav Modifier"));

	TriggerVolume=CreateDefaultSubobject<UBoxComponent>(FName("Trigger Box"));
	TriggerVolume->SetupAttachment(SceneRoot);
	TriggerVolume->SetCanEverAffectNavigation(false);

}

// Called when the game starts or when spawned
void AUnlockables::BeginPlay()
{
	Super::BeginPlay();
	
	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this,&AUnlockables::OnBeginOverlap);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this,&AUnlockables::OnEndOverlap);

	Character = Cast<AFPS_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}

void AUnlockables::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Character->InteractPressed.AddUniqueDynamic(this,&AUnlockables::Unlock);
}

void AUnlockables::OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,int32 OtherBodyIndex)
{
	Character->InteractPressed.Remove(this,FName("Unlock"));
}

void AUnlockables::Unlock()
{
	if (Character->GetPoints()>=UnlockableCost)
	{
		Character->SetPoints(Character->GetPoints()-UnlockableCost);
		Character->UpdateMainHud.Broadcast();
		OnUnlock();
	}
}

void AUnlockables::OnUnlock()
{
	
}

// Called every frame
void AUnlockables::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

