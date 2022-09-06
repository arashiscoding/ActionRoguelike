// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "SGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class ASAICharacter;

UCLASS()
class ACTIONROGUELIKE_API ASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameModeBase();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	float SpawnBotTimerInterval{2.0f};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UEnvQuery* SpawnBotQuery{};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TSubclassOf<ASAICharacter> MinionClass{};

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UCurveFloat* CurveFloat_SpawnBotDifficulty{};
	
protected:
	FTimerHandle TimerHandle_SpawnBot;

protected:
	virtual void StartPlay() override;
	
	void SpawnBot();

	UFUNCTION()
	void OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};
