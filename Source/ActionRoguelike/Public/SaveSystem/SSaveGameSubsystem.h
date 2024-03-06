// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SSaveGameSubsystem.generated.h"

class USSaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveGameSignature, class USSaveGame*, SaveObject);

/**
 * Handles all saving/loading of game state and player data including transform
 */
UCLASS(meta=(DisplayName="SaveGame System")) // DisplayName creates prettier Blueprint node
class ACTIONROGUELIKE_API USSaveGameSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	/** Name of slot to save/load to disk. Filled by SaveGameSettings (can be overriden from GameMode's InitGame()) */
	FString SaveSlotName;

	UPROPERTY()
	USSaveGame* SaveGameObject;
	
	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameLoaded;

	UPROPERTY(BlueprintAssignable)
	FOnSaveGameSignature OnSaveGameWritten;

public:
	/** Initialize Subsystem, good moment to load in SaveGameSettings variables */
	void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/** Restore serialized data from PlayerState into player */
	void HandleStartingNewPlayer(AController* NewPlayer);

	/** Change slot name, will be used next time we load/save data */
	UFUNCTION(BlueprintCallable)
	void SetSlotName(FString NewSlotName);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	/** Load from disk, optional slot name */
	void LoadSaveGame(FString InSlotName = "");

private:
	void SaveGameDelegateFunction(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
