// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/SAction.h"
#include "ActionSystem/SActionComponent.h"
#include "Net/UnrealNetwork.h"

void USAction::Initialize(USActionComponent* NewActionComp)
{
	ActionComp = NewActionComp;
}

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

	RepData.bIsRunning = true;
	RepData.Instigator = Instigator;
}

void USAction::StopAction_Implementation(AActor* Instigator)
{
	UE_LOG(LogTemp, Log, TEXT("Action stopped: %s"), *GetNameSafe(this));

	USActionComponent* Comp = GetOwningComponent();
	Comp->ActiveGameplayTags.RemoveTags(GrantsTags);

	RepData.bIsRunning = false;
	RepData.Instigator = Instigator;
}

/**
 * GetWorld gets called a lot by the engine when we are editing components and at that point,
 * the engine didn't set Outer as we do in SActionComponent when we create an Action.
 * So Outer can be anything.
 * In UE4 replication system, the engine will set the Outer to PlayerCharacter for cliens!
 */
UWorld* USAction::GetWorld() const
{
	AActor* Actor = Cast<AActor>(GetOuter());
	if(Actor)
	{
		return Actor->GetWorld();
	}
	return nullptr;
}

bool USAction::IsRunning() const
{
	return RepData.bIsRunning;
}

USActionComponent* USAction::GetOwningComponent() const
{
	return ActionComp;
}

void USAction::OnRep_RepData()
{
	if(RepData.bIsRunning)
	{
		StartAction(RepData.Instigator);
	}
	else
	{
		StopAction(RepData.Instigator);
	}
}

void USAction::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USAction, RepData);
	DOREPLIFETIME(USAction, ActionComp);
}
