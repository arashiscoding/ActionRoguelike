// Fill out your copyright notice in the Description page of Project Settings.

#include "Powerup/SPowerup_Credit.h"
#include "Framework/SPlayerState.h"

void ASPowerup_Credit::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}

	ASPlayerState* SPlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
	if(ensure(SPlayerState))
	{
		SPlayerState->AddCredits(CreditsToGive);

		DisablePowerup();
	}
}
