// Fill out your copyright notice in the Description page of Project Settings.

#include "SGameModeBase.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SPlayerState.h"
#include "SSaveGame.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Powerup/SPowerupActor.h"


static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ara.SpawnBots"), true, TEXT("Enable spawning of bots."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	LoadSaveGame();
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameModeBase::SpawnBot, SpawnBotTimerInterval, true);

	if(PowerupClasses.Num() > 0)
	{
		UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnPowerupQuery, this, EEnvQueryRunMode::AllMatching, nullptr);
		if(ensureMsgf(QueryInstance, TEXT("Add SpawnPowerup to SGameModeBase")))
		{
			QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnPowerupQueryFinished);
		}
	}
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(SaveGameObject);
	}
}

void ASGameModeBase::OnSpawnPowerupQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("SGameMode | Spawn Powerup EQS query failed!")));
		return;
	}

	TArray<FVector> Locations{};
	QueryInstance->GetQueryResultsAsLocations(Locations);

	// Keep used locations to easily check distance between points
	TArray<FVector> UsedLocations{};
	
	int32 SpawnCounter{};
	
	while (SpawnCounter < DesiredPowerupCount && Locations.Num() > 0)
	{
		int32 RandomLocationIndex = FMath::RandRange(0, Locations.Num()-1);

		FVector PickedLocation = Locations[RandomLocationIndex];

		Locations.RemoveAt(RandomLocationIndex);

		if(UsedLocations.Contains(PickedLocation))
		{
			continue;
		}

		bool bDidFailOnDistance{false};
		
		for(FVector UsedLocation : UsedLocations)
		{
			float Distance = (UsedLocation - PickedLocation).Size();
			if(Distance < RequiredPowerupDistance)
			{
				bDidFailOnDistance = true;
				break;
			}
		}
		
		if(bDidFailOnDistance)
		{
			continue;
		}

		int32 RandomPowerupClassIndex = FMath::RandRange(0, PowerupClasses.Num()-1);
		
		GetWorld()->SpawnActor<ASPowerupActor>(PowerupClasses[RandomPowerupClassIndex], PickedLocation, FRotator::ZeroRotator);

		SpawnCounter++;
		UsedLocations.Emplace(PickedLocation);
	}
}

void ASGameModeBase::SpawnBot()
{
	if(!CVarSpawnBots.GetValueOnGameThread())
	{
		//UE_LOG(LogTemp, Log, TEXT("SGameMode | Bot spawning disabled via cvar 'ara.SpawnBots'"));
		return;
	}
	
	int32 NumberOfAliveBots{};
	for(TActorIterator<ASAICharacter> It{GetWorld()}; It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(Bot);
		if(AttributeComp && AttributeComp->IsAlive())
		{
			NumberOfAliveBots++;
		}
	}

	//UE_LOG(LogTemp, Log, TEXT("SGameMode | Alive Bots: %d"), NumberOfAliveBots);
	
	if(CurveFloat_SpawnBotDifficulty)
	{
		MaxBotCount = CurveFloat_SpawnBotDifficulty->GetFloatValue(GetWorld()->TimeSeconds);
	}
	
	if(NumberOfAliveBots >= MaxBotCount)
	{
		//UE_LOG(LogTemp, Log, TEXT("SGameMode | At maximum bot capacity. Skipping bot spawn."));
		return;
	}
	
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	if(ensureMsgf(QueryInstance, TEXT("Add SpawnBotQuery to SGameModeBase")))
	{
		QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnSpawnBotQueryFinished);
	}
}

void ASGameModeBase::OnSpawnBotQueryFinished(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if(QueryStatus != EEnvQueryStatus::Success)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("SGameMode | Spawn Bot EQS query failed!")));
		return;
	}
	
	TArray<FVector> Locations{};
	QueryInstance->GetQueryResultsAsLocations(Locations);
	
	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<ASAICharacter>(MinionClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::KillAllBots()
{
	for(TActorIterator<ASAICharacter> It{GetWorld()}; It; ++It)
	{
		ASAICharacter* Bot = *It;

		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(Bot);
		if(AttributeComp && AttributeComp->IsAlive())
		{
			AttributeComp->Kill(this);
		}
	}
}

void ASGameModeBase::OnActorKilled(AActor* VictimActor, AActor* KillerActor)
{
	UE_LOG(LogTemp, Log, TEXT("SGameMode | OnActorKilled: Victim: %s, Killer: %s"), *GetNameSafe(VictimActor), *GetNameSafe(KillerActor));

	// Respawn Players after delay
	ASCharacter* Player = Cast<ASCharacter>(VictimActor);
	if(Player)
	{
		FTimerHandle TimerHandle_RespawnPlayer{};
		
		FTimerDelegate Delegate{};
		Delegate.BindUFunction(this, "RespawnPlayerDelayed", Player->GetController());

		float PlayerRespawnDelay{2.0f};
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnPlayer, Delegate, PlayerRespawnDelay, false);
	}

	
	// Give Credits for kill
	ASCharacter* KillerPlayer = Cast<ASCharacter>(KillerActor);
	if(KillerPlayer)
	{
		ASPlayerState* SPlayerState = KillerPlayer->GetPlayerState<ASPlayerState>();
		if(ensure(SPlayerState))
		{
			SPlayerState->AddCredits(CreditsPerKill);
		}
	}
}

void ASGameModeBase::RespawnPlayerDelayed(AController* Controller)
{
	if(ensure(Controller))
	{
		Controller->UnPossess();

		RestartPlayer(Controller);
	}
}

void ASGameModeBase::WriteSaveGame()
{
	for(int32 i{0}; i<GameState->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GameState->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(SaveGameObject);
			break; // single player only at this point
		}
	}
	
	UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
}

void ASGameModeBase::LoadSaveGame()
{
	if(!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		SaveGameObject = Cast<USSaveGame>(UGameplayStatics::CreateSaveGameObject(USSaveGame::StaticClass()));
	
		UE_LOG(LogTemp, Warning, TEXT("Created new SaveGame data."));
		return;
	}
	
	SaveGameObject = Cast<USSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if(!SaveGameObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load SaveGame data!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Loaded SaveGame data."));
}