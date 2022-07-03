// Fill out your copyright notice in the Description page of Project Settings.

#include "AI_Character.h"

#include "FPS_Character.h"
#include "ProjectZGameModeBase.h"
#include "Zombie_AI_Controller.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Damage.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AAI_Character::AAI_Character()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AAI_Character::BeginPlay()
{
	Super::BeginPlay();

	PlayerRef=Cast<AFPS_Character>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	GameModeRef =Cast<AProjectZGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

float AAI_Character::SetDamage(float Damage, float CriticalHitChance, float CriticalHitModifier,const FHitResult& HitResult) const
{
	float LocalDamage = Damage;
	bool bIsCriticalHit;

	if (CriticalHitChance >= FMath::RandRange(0,100))
	{
		bIsCriticalHit = true;
		LocalDamage *= CriticalHitModifier;
	}
	else
	{ 
		bIsCriticalHit = false;
	}

	LocalDamage = LocalDamage * HitResult.PhysMaterial->DestructibleDamageThresholdScale;
	return LocalDamage; 
}

float AAI_Character::SetRadialDamage(float Damage, float Radius, const FHitResult& HitResult, const FVector& ExplosiveLocation) const
{
	float LocalDamage = Damage;

	const float Distance = (GetActorLocation() - ExplosiveLocation).Size();

	UE_LOG(LogTemp,Warning,TEXT("Distance is : %f"),Distance)

	LocalDamage = (1 - (Distance / Radius)) * LocalDamage;
	LocalDamage = FMath::Clamp(LocalDamage,0.0f, Damage);

	return LocalDamage;
}

bool AAI_Character::UpdateHealth(float Damage)
{
	bool bLocalIsDead=false;
	CurrentHealth -= Damage;
	if (CurrentHealth<=0)
	{
		bLocalIsDead = true;
		CurrentHealth = 0;
	}
	return bLocalIsDead;
}

void AAI_Character::TakeDamage(const FAmmoData& AmmoData, float CriticalHitModifier,
	const FHitResult& HitResult)
{
	if (!bIsDead)
	{
		UAISense_Damage::ReportDamageEvent
		    (
			GetWorld(),
			this,
			UGameplayStatics::GetPlayerPawn(GetWorld(),0),0,HitResult.Location,
			GetActorLocation()
			); 
		
		const float DamageTaken = SetDamage(AmmoData.Damage, AmmoData.CriticalHitChance, CriticalHitModifier, HitResult);
		PlayerRef->SetPoints(PlayerRef->GetPoints()+DamageTaken);
		GameModeRef->SetTotalScore(GameModeRef->GetTotalScore()+DamageTaken);
		
		bIsDead = UpdateHealth(DamageTaken);

		if (bIsDead)
		{
			Die();
		}

		EventTakeDamage.Broadcast(DamageTaken,HitResult.ImpactPoint);
	}
}

void AAI_Character::TakeRadialDamage(const FAmmoData& AmmoData, float CriticalHitModifier,const FHitResult& HitResult, const FVector& ExplosiveLocation)
{
	if (!bIsDead)
	{
		UAISense_Damage::ReportDamageEvent
			(
			GetWorld(),
			this,
			UGameplayStatics::GetPlayerPawn(GetWorld(),0),0,HitResult.Location,
			GetActorLocation()
			);
		
		const float DamageTaken = SetRadialDamage(AmmoData.Damage,AmmoData.DamageRadius, HitResult,ExplosiveLocation);
		PlayerRef->SetPoints(PlayerRef->GetPoints()+DamageTaken);
		GameModeRef->SetTotalScore(GameModeRef->GetTotalScore()+DamageTaken);
		
		bIsDead = UpdateHealth(DamageTaken);
 
		if (bIsDead)
		{
			Die();
		}

		EventTakeDamage.Broadcast(DamageTaken,HitResult.ImpactPoint);
	}
}

void AAI_Character::Melee()
{
	if (AttackMontage)
	{
		//Play attack animation
		PlayAnimMontage(AttackMontage);

		//Add Damage after given time
		GetWorldTimerManager().SetTimer(AddDamageTimer,this,&AAI_Character::AddMeleeDamage,1.26,false);
	}
}

void AAI_Character::AddMeleeDamage()
{
	//Object types to trace by
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	//Ignore self
	TArray<AActor*> IgnoreActors;
	IgnoreActors.Init(this, 1);

	//Start loc and end loc (from Ai to the distance specified)
	FVector StartLoc= GetActorLocation();
	FVector EndLoc= StartLoc + GetActorForwardVector()*SphereDistanceMultiplier;

	FHitResult Hit;
	
	//Sphere trace to apply damage if player is hit
	const bool bHit =UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(),StartLoc,EndLoc,SphereRadius,ObjectTypes,false,IgnoreActors,EDrawDebugTrace::None,Hit,true);

	//Apply damage if the actor has a take damage interface
	if (bHit)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor)
		{
			ITakeDamage* TakeDamageInterface = Cast<ITakeDamage>(HitActor);
			if (TakeDamageInterface)
			{
				TakeDamageInterface->TakeMeleeDamage(AttackDamage);
			}
		}
	}
}

void AAI_Character::Die() 
{
	//Reduce the number of alive zombies
	
	GameModeRef->SetZombiesAlive(GameModeRef->GetZombiesAlive()-1);
	GameModeRef->UpdateRoundUI.Broadcast(GameModeRef->GetCurrentRound(),GameModeRef->GetZombiesAlive());
	GameModeRef->IsRoundOver();
	GameModeRef->SetZombiesKilled(GameModeRef->GetZombiesAlive()+1);

	//Simulate RagDoll physics
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle DestroyHandle;
	GetWorldTimerManager().SetTimer(DestroyHandle,[&]
	{
		Destroy();
	},3,false);
}

// Called every frame
void AAI_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AAI_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

