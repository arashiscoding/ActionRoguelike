// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction.h"

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action started: %s"), *GetNameSafe(this));
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action stopped: %s"), *GetNameSafe(this));
}

/**
 * GetWorld gets called a lot by the engine when we are editing components and at that point,
 * the engine dind't set Outer as we do in SActionComponent when we create an Action.
 * So it can be anything.
 */
UWorld* USAction::GetWorld() const
{
	UActorComponent* OuterComp = Cast<UActorComponent>(GetOuter());
	if(OuterComp)
	{
		return OuterComp->GetWorld();
	}
	return nullptr;
}
