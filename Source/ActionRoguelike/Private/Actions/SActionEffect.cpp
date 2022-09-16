// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionEffect.h"
#include "Actions/SActionComponent.h"

USActionEffect::USActionEffect()
{
	bAutoStart = true;
}

void USActionEffect::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	if(Duration > 0.0f)
	{
		FTimerDelegate Delegate{};
		Delegate.BindUFunction(this, "StopAction", Instigator);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Duration, Delegate, Duration, false);
	}

	if(Period > 0.0f)
	{
		FTimerDelegate Delegate{};
		Delegate.BindUFunction(this, "ExecutePeriodicEffect", Instigator);
		
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_Period, Delegate, Period, true);
	}
	
}

void USActionEffect::StopAction_Implementation(AActor* Instigator)
{
	if(GetWorld()->GetTimerManager().GetTimerRemaining(TimerHandle_Period) <= KINDA_SMALL_NUMBER)
	{
		ExecutePeriodicEffect(Instigator);
	}
	
	Super::StopAction_Implementation(Instigator);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Duration);
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle_Period);

	USActionComponent* Comp = GetOwningComponent();
	if(Comp)
	{
		Comp->RemoveAction(this);
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Green, TEXT("Buff Stopped & Removed"));
}

void USActionEffect::ExecutePeriodicEffect_Implementation(AActor* Instigator)
{
	GEngine->AddOnScreenDebugMessage(-1, 6.0f, FColor::Red, TEXT("Buff"));
}
