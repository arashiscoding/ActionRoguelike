// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthPotion.h"
#include "SAttributeComponent.h"

ASHealthPotion::ASHealthPotion()
{
	
}

void ASHealthPotion::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!ensure(InstigatorPawn)) { return; }
	
	USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(InstigatorPawn->GetComponentByClass(USAttributeComponent::StaticClass()));
	if(ensure(AttributeComponent) && AttributeComponent->IsDamaged())
	{
		if(ShouldFullyHeal)
		{
			AttributeComponent->ApplyHealthChange(AttributeComponent->GetMaxHealth());
		}
		else
		{
			AttributeComponent->ApplyHealthChange(HealingAmount);
		}
		
		DisablePowerup();
	}
}