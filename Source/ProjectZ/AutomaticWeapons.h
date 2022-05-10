// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitscanWeapons.h"
#include "AutomaticWeapons.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTZ_API AAutomaticWeapons : public AHitscanWeapons
{
	GENERATED_BODY()
	
public:
	AAutomaticWeapons();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

private:

	FTimerHandle AutoFireHandle;
	FTimerHandle RecoilResetHandle;

	void WeaponFire() override;
	void StopFire() override;

	virtual void StartPlayingTimeLine() override;

	void ResetRecoilTimeline();

	virtual void AddRecoilPitch(float value) override;
	virtual void AddRecoilYaw(float value) override;
	virtual void RevertRecoil() override;

	void AutoFire();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float AutomaticFireRate = 0.1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Property")
	float ResetRecoilRate=0.2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Property")
	bool bIsRecoilReset = true;
};
