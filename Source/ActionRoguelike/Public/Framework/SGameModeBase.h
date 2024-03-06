// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class USSaveGame;
class UEnvQuery;
class ASAICharacter;
class ASPowerupActor;
class UDataTable;

USTRUCT(BlueprintType)
struct FMonsterInfoTableRow : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FPrimaryAssetId MonsterAssetId{};

	/* Chance to pick this monster | Not yet implemented! */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Weight{};

	/* Points required by GameMode to spawn this unit | Not yet implemented! */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SpawnCost{};

	/* Amount of credits awarded to killer of this unit | Not yet implemented! */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float KillReward{};
};

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameModeBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UDataTable* MonsterDataTable{};
	
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotTimerInterval{2.0f};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery{};

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
	
	void OnSpawnPowerupQueryFinished(TSharedPtr<FEnvQueryResult> Result);
	
	void SpawnBot();
	void OnSpawnBotQueryFinished(TSharedPtr<FEnvQueryResult> Result);

	UFUNCTION()
	void OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation);

	UFUNCTION(Exec)
	void KillAllBots();

	UFUNCTION()
	void RespawnPlayerDelayed(AController* Controller);

public:
	virtual void OnActorKilled(AActor* VictimActor, AActor* KillerActor);
};
