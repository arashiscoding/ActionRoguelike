// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup/SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn))
	{
		return;
	}
	
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComp(InstigatorPawn);
	if(ensure(AttributeComponent) && AttributeComponent->IsDamaged())
	{
		ASPlayerState* SPlayerState = InstigatorPawn->GetPlayerState<ASPlayerState>();
		if(ensure(SPlayerState) && SPlayerState->RemoveCredits(PowerupPrice))
		{
			if(ShouldFullyHeal)
			{
				AttributeComponent->ApplyHealthChange(this, AttributeComponent->GetMaxHealth());
			}
			else
			{
				AttributeComponent->ApplyHealthChange(this, HealingAmount);
			}
		
			DisablePowerup();
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red, TEXT("Insufficient Funds!"));
		}
	}
}
