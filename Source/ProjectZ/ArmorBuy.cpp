// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorBuy.h"

#include "FPS_Character.h"

AArmorBuy::AArmorBuy()
{
	Cube=CreateDefaultSubobject<USkeletalMeshComponent>(FName("Cube"));
	SetRootComponent(SceneRoot);
	Cube->SetupAttachment(RootComponent);
	
	Cylinder=CreateDefaultSubobject<USkeletalMeshComponent>(FName("Cylinder"));
	Cylinder->SetupAttachment(Cube);
}

void AArmorBuy::OnUnlock()
{
	Character->SetCurrentArmor(100.0f);
}

void AArmorBuy::Unlock()
{
	if (Character->GetPoints()>=UnlockableCost&&Character->GetCurrentArmor()<100.0f)
	{
		Character->SetPoints(Character->GetPoints()-UnlockableCost);
		Character->UpdateMainHud.Broadcast();
		OnUnlock();
	}
}
