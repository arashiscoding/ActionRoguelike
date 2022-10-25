// Fill out your copyright notice in the Description page of Project Settings.

#include "SaveSystem/SSaveGameSubsystem.h"
#include "EngineUtils.h"
#include "SGameplayInterface.h"
#include "SPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/GameplayStatics.h"
#include "SaveSystem/SSaveGame.h"
#include "SaveSystem/SSaveGameSettings.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"


void USSaveGameSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const USSaveGameSettings* SGSettings = GetDefault<USSaveGameSettings>();
	// Access defaults from DefaultGame.ini
	SaveSlotName = SGSettings->SaveSlotName;

	// Make sure it's loaded into memory .Get() only resolves if already loaded previously elsewhere in code
	UDataTable* DummyTable = SGSettings->DummyTablePath.LoadSynchronous();
	//DummyTable->GetAllRows() // We don't need this table for anything, just an content reference example
}

void USSaveGameSubsystem::HandleStartingNewPlayer(AController* NewPlayer)
{
	ASPlayerState* PS = NewPlayer->GetPlayerState<ASPlayerState>();
	if(PS)
	{
		PS->LoadPlayerState(SaveGameObject);
	}
}

void USSaveGameSubsystem::SetSlotName(FString NewSlotName)
{
	if (NewSlotName.Len() == 0)
	{
		return;
	}

	SaveSlotName = NewSlotName;
}

void USSaveGameSubsystem::WriteSaveGame()
{
	AGameStateBase* GS = GetWorld()->GetGameState();
	if (GS == nullptr)
	{
		// Warn about failure to save?
		return;
	}

	// Iterate all player states, we don't have proper ID to match yet (requires Steam or EOS)
	for(int32 i{0}; i<GS->PlayerArray.Num(); i++)
	{
		ASPlayerState* PS = Cast<ASPlayerState>(GS->PlayerArray[i]);
		if(PS)
		{
			PS->SavePlayerState(SaveGameObject);
			break; // single player only at this point
		}
	}

	SaveGameObject->SavedActors.Empty();

	for(TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if(!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}
		
		FActorSaveData ActorSaveData;
		ActorSaveData.ActorName = Actor->GetName();
		ActorSaveData.Transform = Actor->GetActorTransform();

		FMemoryWriter MemoryWriter(ActorSaveData.ByteData);

		// Pass the array to fill with data from Actor
		FObjectAndNameAsStringProxyArchive Ar(MemoryWriter, true);

		// Find only variables with UPROPERTY(SaveGame)
		Ar.ArIsSaveGame = true;

		// Converts Actor's variables with UPROPERTY(SaveGame) into binary array
		Actor->Serialize(Ar);
		
		SaveGameObject->SavedActors.Emplace(ActorSaveData);
	}

	/* Synchronous Saving */
	//UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveSlotName, 0);
	//OnSaveGameWritten.Broadcast(SaveGameObject);

	/* Asynchronous Saving */
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	SavedDelegate.BindUObject(this, &USSaveGameSubsystem::SaveGameDelegateFunction);

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("New Starting to save"));
	UGameplayStatics::AsyncSaveGameToSlot(SaveGameObject, SaveSlotName, 0, SavedDelegate);
}

void USSaveGameSubsystem::SaveGameDelegateFunction(const FString& SlotName, const int32 UserIndex, bool bSuccess)
{
	if(bSuccess)
	{
		OnSaveGameWritten.Broadcast(SaveGameObject);
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("New Saved Successfully!"));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("New Save Failed!"));
	}
}

void USSaveGameSubsystem::LoadSaveGame(FString InSlotName)
{
	// Update slot name first if specified, otherwise keeps default name
	SetSlotName(InSlotName);
	
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

	for(TActorIterator<AActor> It(GetWorld()); It; ++It)
	{
		AActor* Actor = *It;
		if(!Actor->Implements<USGameplayInterface>())
		{
			continue;
		}

		for(FActorSaveData ActorSaveData : SaveGameObject->SavedActors)
		{
			if(ActorSaveData.ActorName == Actor->GetName())
			{
				Actor->SetActorTransform(ActorSaveData.Transform);

				FMemoryReader MemoryReader(ActorSaveData.ByteData);

				// Pass the array to fill with data from Actor
				FObjectAndNameAsStringProxyArchive Ar(MemoryReader, true);

				// Find only variables with UPROPERTY(SaveGame)
				Ar.ArIsSaveGame = true;

				// Convert binary array back into Actor's variables with UPROPERTY(SaveGame)
				Actor->Serialize(Ar);

				ISGameplayInterface::Execute_OnActorLoaded(Actor);
				
				break;
			}
		}
	}

	OnSaveGameLoaded.Broadcast(SaveGameObject);
}
