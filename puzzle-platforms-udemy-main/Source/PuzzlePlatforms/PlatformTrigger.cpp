// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "MovingPlatform.h"
#include "Components/BoxComponent.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerVolume = CreateDefaultSubobject<UBoxComponent>(FName("TriggerVolume"));
	if (!ensure(TriggerVolume != nullptr))
		return;
		
	RootComponent = TriggerVolume;

	TriggerVolume->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
	TriggerVolume->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// here, we do a HasAuthority check because we only need this to run once. It is an object in the environment, so its logic can be exclusively owned by server
	if (HasAuthority())
	{
		ActorsOnPlatform += 1;
		if (ActorsOnPlatform == 1)
		{
			// could also for (AMovingPlatform* Platform : PlatformsToTrigger)
			for (int i = 0; i < PlatformsToTrigger.Num(); i++)
			{
				PlatformsToTrigger[i]->AddActiveTrigger();
			}
		}
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (HasAuthority())
	{
		ActorsOnPlatform -= 1;
		if (ActorsOnPlatform == 0)
		{
			for (int i = 0; i < PlatformsToTrigger.Num(); i++)
			{
				PlatformsToTrigger[i]->RemoveActiveTrigger();
			}			
		}
	}
}

