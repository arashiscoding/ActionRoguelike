// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
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