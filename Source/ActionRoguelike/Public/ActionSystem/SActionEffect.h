// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/SAction.h"
#include "SActionEffect.generated.h"


UCLASS()
class ACTIONROGUELIKE_API USActionEffect : public USAction
{
	GENERATED_BODY()

public:
	USActionEffect();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Duration{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effect")
	float Period{};
	
	FTimerHandle TimerHandle_Duration{};
	FTimerHandle TimerHandle_Period{};

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;
	virtual void StopAction_Implementation(AActor* Instigator) override;

	UFUNCTION(BlueprintCallable)
	float GetTimeRemaining() const;
	
protected:
	UFUNCTION(BlueprintNativeEvent, Category = "Effect")
	void ExecutePeriodicEffect(AActor* Instigator);
};
