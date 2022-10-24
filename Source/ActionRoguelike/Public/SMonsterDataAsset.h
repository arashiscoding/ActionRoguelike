// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SMonsterDataAsset.generated.h"

class ASAICharacter;
class USAction;

UCLASS()
class ACTIONROGUELIKE_API USMonsterDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TSubclassOf<ASAICharacter> MonsterClass{};

	/* Actions/Effects to grant this monster | Not yet implemented! */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Spawn Info")
	TArray<TSubclassOf<USAction>> Actions{};

	/* Not yet implemented! */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	UTexture2D* Icon{};
};
