// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Unlockables.h"
#include "AutomaticDoor.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTZ_API AAutomaticDoor : public AUnlockables
{
	GENERATED_BODY()

	AAutomaticDoor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftDoor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightDoor;

	float AllAddedOffset=0.0f;
	
	FTimerHandle Handle;
	
	virtual void OnUnlock() override;

protected:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Spawner")
	class AZombieSpawner* TheSpawner;
	
};
