// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/SAction.h"
#include "ActionSystem/SActionComponent.h"

bool USAction::CanStart_Implementation(AActor* Instigator)
{
	if(IsRunning())
	{
		return false;
	}
	
	USActionComponent* Comp = GetOwningComponent();
	
	if(Comp->ActiveGameplayTags.HasAny(BlockedTags))
	{
		UE_LOG(LogTemp, Warning, TEXT("Action blocked from starting: %s"), *GetNameSafe(this));
		return false;
	}
	return true;
}

void USAction::StartAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action started: %s"), *GetNameSafe(this));

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.AppendTags(GrantsTags);

	bIsRunning = true;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action stopped: %s"), *GetNameSafe(this));

	ensureAlways(bIsRunning);

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	bIsRunning = false;
}

/**
 * GetWorld gets called a lot by the engine when we are editing components and at that point,
 * the engine didn't set Outer as we do in SActionComponent when we create an Action.
 * So Outer can be anything.
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

bool USAction::IsRunning() const
{
	return bIsRunning;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return Cast<USActionComponent>(GetOuter());
}