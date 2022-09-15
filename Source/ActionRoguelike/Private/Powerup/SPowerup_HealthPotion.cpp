// Fill out your copyright notice in the Description page of Project Settings.


#include "Powerup/SPowerup_HealthPotion.h"
#include "SAttributeComponent.h"

ASPowerup_HealthPotion::ASPowerup_HealthPotion()
{
	
}

void ASPowerup_HealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	Super::Interact_Implementation(InstigatorPawn);
	
	if(!ensure(InstigatorPawn)) { return; }
	
	USAttributeComponent* AttributeComponent = USAttributeComponent::GetAttributeComp(InstigatorPawn);
	if(ensure(AttributeComponent) && AttributeComponent->IsDamaged())
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
}
