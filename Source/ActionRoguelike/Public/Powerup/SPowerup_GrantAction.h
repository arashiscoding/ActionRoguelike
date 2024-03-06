// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Powerup/SPowerupActor.h"
#include "SPowerup_GrantAction.generated.h"

class USAction;

UCLASS()
class ACTIONROGUELIKE_API ASPowerup_GrantAction : public ASPowerupActor
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Powerup")
	TSubclassOf<USAction> ActionToGrant{};
	
protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
