// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup/SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"
#include "SPlayerState.h"

// With this, now instead of NSLOCTEXT, we can use LOCTEXT for defining texts without specifying namespace everytime
#define LOCTEXT_NAMESPACE "InteractableActors"

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
				AttributeComponent->ApplyHealthChange(this, AttributeComponent->GetHealthMax());
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

FText ASPowerup_HealthPotion::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComp(InstigatorPawn);
	if(ensure(AttributeComponent) && !AttributeComponent->IsDamaged())
	{
		return LOCTEXT("HealthPotion_FullHealth", "Already at full health");
	}

	return FText::Format(LOCTEXT("HealthPotion_Cost", "Costs {0} credits. Restores {1} health"), PowerupPrice, HealingAmount);
}

#undef LOCTEXT_NAMESPACE