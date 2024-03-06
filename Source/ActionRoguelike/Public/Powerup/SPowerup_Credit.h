// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Powerup/SPowerupActor.h"
#include "SPowerup_Credit.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASPowerup_Credit : public ASPowerupActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Credit")
	int32 CreditsToGive{100};
	
protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
