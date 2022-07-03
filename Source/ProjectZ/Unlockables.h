// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Unlockables.generated.h"

UCLASS()
class PROJECTZ_API AUnlockables : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUnlockables();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SceneRoot", meta = (AllowPrivateAccess = "true"))
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Navigation", meta = (AllowPrivateAccess = "true"))
	class UNavModifierComponent* NavModifier;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Trigger", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerVolume;

	UFUNCTION()
	void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void Unlock();

	UPROPERTY()
	class AFPS_Character* Character;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Property")
	FName UnlockableName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Property")
	int32 UnlockableCost;

	
	virtual void OnUnlock();
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
