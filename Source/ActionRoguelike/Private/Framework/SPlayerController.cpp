// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/SPlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

void ASPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	
	BlueprintBeginPlayingState();
}

void ASPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	OnPawnChanged.Broadcast(InPawn);
}

void ASPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	OnPlayerStateChanged.Broadcast(PlayerState);
}

void ASPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	InputComponent->BindAction("PauseMenu", IE_Pressed, this, &ASPlayerController::TogglePauseMenu);
}

void ASPlayerController::TogglePauseMenu()
{
	if(PauseMenuInstance && PauseMenuInstance->IsInViewport())
	{
		PauseMenuInstance->RemoveFromParent();
		PauseMenuInstance = nullptr;

		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly{});

		// Single-player only
		if(GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, false);
		}

		return;
	}
	
	PauseMenuInstance = CreateWidget<UUserWidget>(this, PauseMenuClass);
	if(PauseMenuInstance)
	{
		PauseMenuInstance->AddToViewport(100);

		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly{});

		// Single-player only
		if(GetWorld()->IsNetMode(NM_Standalone))
		{
			UGameplayStatics::SetGamePaused(this, true);
		}
	}
}