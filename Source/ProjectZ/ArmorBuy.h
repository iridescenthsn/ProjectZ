// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unlockables.h"
#include "ArmorBuy.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AArmorBuy : public AUnlockables
{
	GENERATED_BODY()

	AArmorBuy();

protected:

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Skeletal mesh",meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* Cube;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Skeletal mesh",meta=(AllowPrivateAccess="true"))
	USkeletalMeshComponent* Cylinder;

	virtual void OnUnlock() override;

	virtual void Unlock() override;
	
};
