// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"
#include "SGameModeBase.h"
#include "Net/UnrealNetwork.h"

USAttributeComponent::USAttributeComponent()
{
	Health = HealthMax;

	SetIsReplicatedByDefault(true);
}

bool USAttributeComponent::ApplyHealthChange(AActor* InstigatorActor, float Delta)
{
	/* "God" console command changes "CanBeDamaged" property of player pawn */
	if(!GetOwner()->CanBeDamaged() && Delta < 0.0f)
	{
		return false;
	}

	float OldHealth = Health;
	
	Health = FMath::Clamp(Health + Delta, 0.0f, HealthMax);
	
	float ActualDelta = Health - OldHealth;
	
	//OnHealthChanged.Broadcast(InstigatorActor, this, Health, ActualDelta);
	if(ActualDelta != 0.0f)
	{
		MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
	}
	
	// Died
	if(ActualDelta < 0.0f && !IsAlive())
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("Hit"));
		
		ASGameModeBase* SGameModeBase = GetWorld()->GetAuthGameMode<ASGameModeBase>();
		if(SGameModeBase)
		{
			SGameModeBase->OnActorKilled(GetOwner(), InstigatorActor);
		}
	}

	/* if ActualDelta would be 0, it means health didn't change.
	 * Either at full health, or has died. */
	return ActualDelta != 0.0f;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;

	Rage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = Rage - OldRage;

	if(ActualDelta != 0.0f)
	{
		OnRageChanged.Broadcast(InstigatorActor, this, Rage, ActualDelta);
	}

	return ActualDelta != 0.0f;
}

float USAttributeComponent::GetHealth() const
{
	return Health;
}

float USAttributeComponent::GetHealthMax() const
{
	return HealthMax;
}

bool USAttributeComponent::IsAlive() const
{
	return Health > 0.0f;
}

bool USAttributeComponent::IsDamaged() const
{
	return Health < HealthMax;
}

float USAttributeComponent::GetRage() const
{
	return Rage;
}

float USAttributeComponent::GetRageMax() const
{
	return RageMax;
}

float USAttributeComponent::GetRagePercentPerDamage() const
{
	return RagePercentPerDamage / 100.0f;
}

bool USAttributeComponent::Kill(AActor* InstigatorActor)
{
	return ApplyHealthChange(InstigatorActor, -GetHealthMax());
}

USAttributeComponent* USAttributeComponent::GetAttributeComp(AActor* FromActor)
{
	if(FromActor)
	{
		return FromActor->FindComponentByClass<USAttributeComponent>();
	}
	return nullptr;
}

bool USAttributeComponent::IsActorAlive(AActor* Actor)
{
	USAttributeComponent* AttributeComp = GetAttributeComp(Actor);
	if(AttributeComp)
	{
		return AttributeComp->IsAlive();
	}
	return false;
}

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewValue, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewValue, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
}