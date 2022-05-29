// Fill out your copyright notice in the Description page of Project Settings.


#include "TakeDamage.h"

// Add default functionality here for any ITakeDamage functions that are not pure virtual.

void ITakeDamage::TakeDamage(const FAmmoData& AmmoData, float CriticalHitModifier, const FHitResult& HitResult)
{
	return;
}

void ITakeDamage::TakeRadialDamage(const FAmmoData& AmmoData, float CriticalHitModifier, const FHitResult& HitResult, const FVector& ExplosiveLocation)
{
	return;
}

void ITakeDamage::TakeMeleeDamage(float Damage)
{
	return;
}
