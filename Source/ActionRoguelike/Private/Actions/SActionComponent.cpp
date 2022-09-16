// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionComponent.h"
#include "Actions/SAction.h"


USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<USAction>& ActionClass : DefaultActions)
	{
		AddAction(GetOwner(), ActionClass);
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);
}

void USActionComponent::AddAction(AActor* Instigator, const TSubclassOf<USAction>& ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);
		if(NewAction->bAutoStart && NewAction->CanStart(Instigator))
		{
			StartActionByName(Instigator, NewAction->ActionName);
		}
	}
}

void USActionComponent::RemoveAction(USAction* ActionToRemove)
{
	if(ensure(ActionToRemove && !ActionToRemove->IsRunning()))
	{
		Actions.Remove(ActionToRemove);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(!Action->CanStart(Instigator))
			{
				FString FailedMsg = FString::Printf(TEXT("Can't start action: %s"), *ActionName.ToString());
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FailedMsg);
				continue;
			}
			
			Action->StartAction(Instigator);
			return true;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Action [%s] wasn't found. Unable to start!"), *ActionName.ToString());
	return false;
}

bool USActionComponent::StopActionByName(AActor* Instigator, FName ActionName)
{
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
			if(Action->IsRunning())
			{
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Action [%s] wasn't found. Unable to stop!"), *ActionName.ToString());
	return false;
}
