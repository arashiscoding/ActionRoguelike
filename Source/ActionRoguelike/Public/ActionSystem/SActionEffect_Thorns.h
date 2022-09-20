// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "ActionSystem/SActionEffect.h"
#include "SActionEffect_Thorns.generated.h"

class USAttributeComponent;

/**
 * This buff will return some fraction of damage dealt, to its causer */
UCLASS()
class ACTIONROGUELIKE_API USActionEffect_Thorns : public USActionEffect
{
	GENERATED_BODY()

public:
	USActionEffect_Thorns();

protected:
	// How much of the damage dealt, should be returned to its causer
	UPROPERTY(EditDefaultsOnly, Category = "Thorns")
	float ReflectFraction{0.2f};

protected:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewValue, float Delta);
};
