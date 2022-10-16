// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USSaveGame;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class ASAICharacter;
class ASPowerupActor;

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameModeBase();

protected:
	UPROPERTY()
	USSaveGame* SaveGameObject{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveGame")
	FString SaveSlotName{"SaveGame01"};

	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotTimerInterval{2.0f};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery{};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<ASAICharacter> MinionClass{};

	/* Increases MaxBotCount as the time passes according to its curve */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* CurveFloat_SpawnBotDifficulty{};

	/* If 'CurveFloat_SpawnBotDifficulty' is not assigned, this value will be used */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
	float MaxBotCount = 10.0f;

	/* Read/write access as we could change this as our difficulty increases */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	int32 CreditsPerKill{50};

	
	UPROPERTY(EditAnywhere, Category = "Powerups")
	UEnvQuery* SpawnPowerupQuery{};

	UPROPERTY(EditAnywhere, Category = "Powerups")
	TArray<TSubclassOf<ASPowerupActor>> PowerupClasses{};

	UPROPERTY(EditAnywhere, Category = "Powerups")
	int32 DesiredPowerupCount{10};
	
	UPROPERTY(EditAnywhere, Category = "Powerups")
	float RequiredPowerupDistance{2000.0f};
	
	
protected:
	FTimerHandle TimerHandle_SpawnBot;

protected:
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;
	
	/* Signals that a player is ready to enter the game, which may start it up */
	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;
	
	UFUNCTION()
	void OnSpawnPowerupQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
	void SpawnBot();

	UFUNCTION()
	void OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	UFUNCTION(Exec)
	void KillAllBots();

	UFUNCTION()
	void RespawnPlayerDelayed(AController* Controller);

public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);

	UFUNCTION(BlueprintCallable, Category = "SaveGame")
	void WriteSaveGame();

	void LoadSaveGame();

private:
	void SaveGameDelegateFunction(const FString& SlotName, const int32 UserIndex, bool bSuccess);
};
