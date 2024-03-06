// Fill out your copyright notice in the Description page of Project Settings.

#include "Framework/SGameModeBase.h"
#include "EngineUtils.h"
#include "SAttributeComponent.h"
#include "SCharacter.h"
#include "SMonsterDataAsset.h"
#include "Framework/SPlayerState.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionSystem/SActionComponent.h"
#include "AI/SAICharacter.h"
#include "Engine/AssetManager.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "Powerup/SPowerupActor.h"
#include "SaveSystem/SSaveGameSubsystem.h"

static TAutoConsoleVariable<bool> CVarSpawnBots(TEXT("ara.SpawnBots"), true, TEXT("Enable spawning of bots."), ECVF_Cheat);

ASGameModeBase::ASGameModeBase()
{
	PlayerStateClass = ASPlayerState::StaticClass();
}

void ASGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
	
	USSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<USSaveGameSubsystem>();
	
	// Optional save slot name (If not provided, falls back to name specified in SaveGameSettings class/INI)
	FString SelectedSaveSlot = UGameplayStatics::ParseOption(Options, "SaveGame");
	SG->LoadSaveGame(SelectedSaveSlot);
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBot, this, &ASGameModeBase::SpawnBot, SpawnBotTimerInterval, true);
	
	if(PowerupClasses.Num() > 0)
	{
		// Run EQS to find potential power-up spawn locations
		FEnvQueryRequest Request(SpawnPowerupQuery, this);
		Request.Execute(EEnvQueryRunMode::AllMatching, this, &ASGameModeBase::OnSpawnPowerupQueryFinished);
	}
}

void ASGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	// Calling before Super:: so we set variables before "BeginPlayingState" is called in PlayerController (Which is where we instantiate UI)
	USSaveGameSubsystem* SG = GetGameInstance()->GetSubsystem<USSaveGameSubsystem>();
	SG->HandleStartingNewPlayer(NewPlayer);
	
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
}

void ASGameModeBase::OnSpawnPowerupQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if(!QueryResult->IsSuccessful())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("SGameMode | Spawn Powerup EQS query failed!")));
		return;
	}

	TArray<FVector> Locations{};
	QueryResult->GetAllAsLocations(Locations);

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
	for(ASAICharacter* Bot : TActorRange<ASAICharacter>(GetWorld()))
	{
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
	
	// Run EQS to find valid spawn location
	FEnvQueryRequest Request(SpawnBotQuery, this);
	Request.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ASGameModeBase::OnSpawnBotQueryFinished);
}

void ASGameModeBase::OnSpawnBotQueryFinished(TSharedPtr<FEnvQueryResult> Result)
{
	FEnvQueryResult* QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		GEngine->AddOnScreenDebugMessage(-1, 10.0f, FColor::Red, FString::Printf(TEXT("SGameMode | Spawn Bot EQS query failed!")));
		return;
	}
	
	TArray<FVector> Locations{};
	QueryResult->GetAllAsLocations(Locations);
	
	if(Locations.IsValidIndex(0) && MonsterDataTable)
	{
		TArray<FMonsterInfoTableRow*> Rows{};
		MonsterDataTable->GetAllRows("", Rows);
		
		// Select random enemy
		const int32 RandomIndex = FMath::RandRange(0, Rows.Num()-1);
		FMonsterInfoTableRow* SelectedRow = Rows[RandomIndex];
		
		if(UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
		{
			LogOnScreen(this, "Loading Monster...", FColor::Green);
			
			TArray<FName> Bundles;
			FStreamableDelegate Delegate = FStreamableDelegate::CreateUObject(this, &ASGameModeBase::OnMonsterLoaded, SelectedRow->MonsterAssetId, Locations[0]);
			AssetManager->LoadPrimaryAsset(SelectedRow->MonsterAssetId, Bundles, Delegate);
		}
	}
}

void ASGameModeBase::OnMonsterLoaded(FPrimaryAssetId LoadedId, FVector SpawnLocation)
{
	LogOnScreen(this, "Finished loading Monster. Spawning now!", FColor::Green);
	
	if(UAssetManager* AssetManager = UAssetManager::GetIfInitialized())
	{
		USMonsterDataAsset* MonsterDataAsset = Cast<USMonsterDataAsset>(AssetManager->GetPrimaryAssetObject(LoadedId));
		if(!MonsterDataAsset)
		{
			return;
		}
		
		AActor* NewBot = GetWorld()->SpawnActor<ASAICharacter>(MonsterDataAsset->MonsterClass, SpawnLocation, FRotator::ZeroRotator);
		if(NewBot)
		{
			// Grant special Actions/Effects, etc.
			if(USActionComponent* ActionComp = USActionComponent::GetActionComp(NewBot))
			{
				for(TSubclassOf<USAction>& ActionClass : MonsterDataAsset->Actions)
				{
					ActionComp->AddAction(NewBot, ActionClass);
				}
			}
		}
	}
}

void ASGameModeBase::KillAllBots()
{
	for(ASAICharacter* Bot : TActorRange<ASAICharacter>(GetWorld()))
	{
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
	if(ASCharacter* Player = Cast<ASCharacter>(VictimActor))
	{
		FTimerHandle TimerHandle_RespawnPlayer{};
		
		FTimerDelegate Delegate{};
		Delegate.BindUFunction(this, "RespawnPlayerDelayed", Player->GetController());

		float PlayerRespawnDelay{2.0f};
		GetWorldTimerManager().SetTimer(TimerHandle_RespawnPlayer, Delegate, PlayerRespawnDelay, false);
	}
	
	// Give Credits for kill
	if(ASCharacter* Player = Cast<ASCharacter>(KillerActor))
	{
		ASPlayerState* SPlayerState = Player->GetPlayerState<ASPlayerState>();
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