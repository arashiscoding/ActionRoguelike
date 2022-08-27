// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health += Delta;
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

