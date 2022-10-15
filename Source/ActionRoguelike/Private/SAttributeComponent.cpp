// Fill out your copyright notice in the Description page of Project Settings.

#include "SAttributeComponent.h"

#include "SCharacter.h"
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
	float NewHealth = FMath::Clamp(Health + Delta, 0.0f, HealthMax);

	float ActualDelta = NewHealth - OldHealth;

	// Is Server?
	if(GetOwner()->HasAuthority())
	{
		Health = NewHealth;

		if(ActualDelta != 0.0f)
		{
			MulticastHealthChanged(InstigatorActor, Health, ActualDelta);
		}

		// Died
		if(ActualDelta < 0.0f && !IsAlive())
		{
			ASGameModeBase* SGameModeBase = GetWorld()->GetAuthGameMode<ASGameModeBase>();
			if(SGameModeBase)
			{
				SGameModeBase->OnActorKilled(GetOwner(), InstigatorActor);
			}
		}
	}

	/* if ActualDelta would be 0, it means health didn't change.
	 * Either at full health, or has died. */
	return ActualDelta != 0.0f;
}

bool USAttributeComponent::ApplyRageChange(AActor* InstigatorActor, float Delta)
{
	float OldRage = Rage;
	float NewRage = FMath::Clamp(Rage + Delta, 0.0f, RageMax);

	float ActualDelta = NewRage - OldRage;

	// Is Server?
	if(GetOwner()->HasAuthority())
	{
		Rage = NewRage;
		
		if(ActualDelta != 0.0f)
		{
			MulticastRageChanged(InstigatorActor, Rage, ActualDelta);
		}
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

void USAttributeComponent::MulticastHealthChanged_Implementation(AActor* InstigatorActor, float NewHealth, float Delta)
{
	OnHealthChanged.Broadcast(InstigatorActor, this, NewHealth, Delta);
}

void USAttributeComponent::MulticastRageChanged_Implementation(AActor* InstigatorActor, float NewRage, float Delta)
{
	OnRageChanged.Broadcast(InstigatorActor, this, NewRage, Delta);
}

void USAttributeComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USAttributeComponent, Health);
	DOREPLIFETIME(USAttributeComponent, HealthMax);
	DOREPLIFETIME(USAttributeComponent, Rage);
	DOREPLIFETIME(USAttributeComponent, RageMax);
}