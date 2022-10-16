// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SPlayerController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPawnChanged, APawn*, NewPawn);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerStateChanged, APlayerState*, NewPlayerState);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> PauseMenuClass{};

	UPROPERTY()
	UUserWidget* PauseMenuInstance{};
	
	UPROPERTY(BlueprintAssignable)
	FOnPawnChanged OnPawnChanged{};

	/* Listen for incoming PlayerState (for clients this may be nullptr when initially joining a game,
	 * afterwards PlayerState won't change again as PlayerControllers maintain the same PlayerState throughout the level) */
	UPROPERTY(BlueprintAssignable)
	FOnPlayerStateChanged OnPlayerStateChanged{};

protected:
	/* Called when PlayerController is ready to begin playing, good moment to initialize things like UI which might be too early in BeginPlay
	 * (esp. in multiplayer clients where not all data such as PlayerState may have been received yet). */
	virtual void BeginPlayingState() override;

	UFUNCTION(BlueprintImplementableEvent)
	void BlueprintBeginPlayingState();
	
	virtual void SetPawn(APawn* InPawn) override;
	virtual void OnRep_PlayerState() override;
	virtual void SetupInputComponent() override;

	UFUNCTION(BlueprintCallable)
	void TogglePauseMenu();
};
