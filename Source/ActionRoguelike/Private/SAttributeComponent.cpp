// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"

USAttributeComponent::USAttributeComponent()
{
	
}

void USAttributeComponent::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
}

bool USAttributeComponent::ApplyHealthChange(float Delta)
{
	Health = FMath::Clamp(Health + Delta, 0.0f, MaxHealth);
	
	OnHealthChanged.Broadcast(nullptr, this, Health, Delta);
	
	return true;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsDamaged() const
{
	return Health < MaxHealth;
}

float USAttributeComponent::GetMaxHealth() const
{
	return MaxHealth;
}

