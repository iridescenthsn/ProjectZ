// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tracer.generated.h"

UCLASS()
class PROJECTZ_API ATracer : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATracer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="Mesh")
	UStaticMeshComponent* TracerMesh;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SceneRoot")
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly,Category="SceneRoot")
	class UProjectileMovementComponent* MovementComponent;
public:
	UProjectileMovementComponent* GetMovementComponent() const {return MovementComponent;}

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
