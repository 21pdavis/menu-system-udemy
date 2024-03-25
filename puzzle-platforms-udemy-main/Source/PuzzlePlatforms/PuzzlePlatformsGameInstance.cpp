// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformsGameInstance.h"

#include "Engine/Engine.h"

UPuzzlePlatformsGameInstance::UPuzzlePlatformsGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Constructing Game Instance!"));
}

void UPuzzlePlatformsGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Initializing Game Instance!"));
}

void UPuzzlePlatformsGameInstance::Host()
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
		return;
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr))
		return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, TEXT("Traveling..."));
	World->ServerTravel("/Game/ThirdPerson/Maps/ThirdPersonMap?listen");
}

void UPuzzlePlatformsGameInstance::Join(const FString& Address)
{
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr))
		return;

	// Note here: *Address is valid on FString& type because of how * operator is defined in FString. Think of it like an FString.GetText() call
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, FString::Printf(TEXT("%s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr))
		return;

	// PlayerController->ClientTravel("/Game/ThirdPerson/Maps/ThirdPersonMap", TRAVEL_Absolute);
	PlayerController->ClientTravel(Address, TRAVEL_Absolute);
}

