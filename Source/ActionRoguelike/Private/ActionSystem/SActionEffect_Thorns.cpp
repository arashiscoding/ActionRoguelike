// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionSystem/SActionEffect_Thorns.h"
#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "ActionSystem/SActionComponent.h"

USActionEffect_Thorns::USActionEffect_Thorns()
{
	// We don't want its periodic effect and want it to always be active
	Duration = 0.0f;
	Period = 0.0f;

	ReflectFraction = 0.2f;
}

void USActionEffect_Thorns::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(Instigator);
	if(AttributeComp)
	{
		AttributeComp->OnHealthChanged.AddDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(Instigator);
	if(AttributeComp)
	{
		AttributeComp->OnHealthChanged.RemoveDynamic(this, &USActionEffect_Thorns::OnHealthChanged);
	}
}

void USActionEffect_Thorns::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta)
{
	AActor* OwningActor = GetOwningComponent()->GetOwner();
	
	if(Delta < 0.0f && OwningActor != InstigatorActor)
	{
		int32 ReflectedDamageAmount = FMath::RoundToInt(Delta * ReflectFraction);
		
		if(ReflectedDamageAmount == 0)
		{
			return;
		}

		ReflectedDamageAmount = FMath::Abs(ReflectedDamageAmount);
		
		USGameplayFunctionLibrary::ApplyDamage(OwningActor, InstigatorActor, ReflectedDamageAmount);
	}
}
