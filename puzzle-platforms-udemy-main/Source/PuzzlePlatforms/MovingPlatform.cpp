// Fill out your copyright notice in the Description page of Project Settings.


#include "MovingPlatform.h"

AMovingPlatform::AMovingPlatform()
{
	// Unity vs. Unreal note: These things might be done in Awake(), but in Unreal we use the constructor.
	
	// have to manually specify that actor ticks
	PrimaryActorTick.bCanEverTick = true;

	// set default move speed.
	MoveSpeed = 100.0f;

	// Important Concept: All actors are static by default. We have to set them to movable.
	SetMobility(EComponentMobility::Movable);
}

void AMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// only the server is allowed to replicate
	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	GlobalStartLocation = GetActorLocation();
	GlobalTargetLocation = GetTransform().TransformPosition(TargetLocation);
}

void AMovingPlatform::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HasAuthority() && ActiveTriggers >= RequiredActiveTriggers)
	{
		const FVector Location = GetActorLocation();

		if (FVector::Dist(Location, GlobalStartLocation) >= FVector::Dist(GlobalStartLocation, GlobalTargetLocation))
		{
			const FVector Temp = GlobalStartLocation;
			GlobalStartLocation = GlobalTargetLocation;
			GlobalTargetLocation = Temp;
		}
		const FVector MoveDirection = (GlobalTargetLocation - GlobalStartLocation).GetSafeNormal();
		const FVector NewLocation = Location + DeltaTime * MoveSpeed * MoveDirection;
		SetActorLocation(NewLocation);
	}
}

void AMovingPlatform::AddActiveTrigger()
{
	ActiveTriggers += 1;
}

void AMovingPlatform::RemoveActiveTrigger()
{
	ActiveTriggers -= 1;
}


