// Fill out your copyright notice in the Description page of Project Settings.


#include "Tower.h"

void ATower::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle FireTimerHandle;
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &ATower::CheckFireCondition, FireRate, true);
}

void ATower::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Tank)
	{
		if (IsInFireRange())
		{
			RotateTurret(Tank->GetActorLocation());
		}
	}
}

void ATower::CheckFireCondition()
{
	if (Tank)
	{
		if (IsInFireRange() && Tank->IsAlive)
		{
			Fire();
		}
	}
}

bool ATower::IsInFireRange()
{
	float DistanceToTarget = FVector::Dist(Tank->GetActorLocation(), GetActorLocation());
	if (DistanceToTarget <= RangeOfAttacks)
	{
		return true;
	}
	return false;
}

void ATower::HandleDestruction()
{
	Super::HandleDestruction();
	Destroy();
}
