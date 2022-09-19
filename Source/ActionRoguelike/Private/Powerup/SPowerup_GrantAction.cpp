// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup/SPowerup_GrantAction.h"
#include "ActionSystem/SAction.h"
#include "ActionSystem/SActionComponent.h"

void ASPowerup_GrantAction::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensureAlways(InstigatorPawn && ActionToGrant))
	{
		return;
	}
	
	USActionComponent* ActionComp = USActionComponent::GetActionComp(InstigatorPawn);
	if(ActionComp)
	{
		if(ActionComp->GetAction(ActionToGrant))
		{
			FString DebugMsg = FString::Printf(TEXT("Action '%s' already exists."), *GetNameSafe(ActionToGrant));
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, DebugMsg);
			return;
		}

		ActionComp->AddAction(this, ActionToGrant);
		DisablePowerup();
	}
}
