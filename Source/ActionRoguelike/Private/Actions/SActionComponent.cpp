// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SActionComponent.h"
#include "Actions/SAction.h"


USActionComponent::USActionComponent()
{
	
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	for(TSubclassOf<USAction>& ActionClass : DefaultActions)
	{
		AddAction(ActionClass);
	}
}

void USActionComponent::AddAction(TSubclassOf<USAction>& ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	USAction* NewAction = NewObject<USAction>(this, ActionClass);
	if(ensure(NewAction))
	{
		Actions.Add(NewAction);
	}
}

bool USActionComponent::StartActionByName(AActor* Instigator, FName ActionName)
{
	for(USAction* Action : Actions)
	{
		if(Action && Action->ActionName == ActionName)
		{
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
			Action->StopAction(Instigator);
			return true;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Action [%s] wasn't found. Unable to stop!"), *ActionName.ToString());
	return false;
}
