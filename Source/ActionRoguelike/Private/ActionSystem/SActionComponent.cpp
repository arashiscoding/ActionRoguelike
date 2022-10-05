// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/SActionComponent.h"
#include "ActionSystem/SAction.h"

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

USAction* USActionComponent::GetAction(TSubclassOf<USAction> ActionClass)
{
	for(USAction* Action : Actions)
	{
		if(Action->IsA(ActionClass))
		{
			return Action;
		}
	}
	
	return nullptr;
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

			// Is client?
			if(!GetOwner()->HasAuthority())
			{
				ServerStartAction(Instigator, ActionName);
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

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

USActionComponent* USActionComponent::GetActionComp(AActor* FromActor)
{
	if(FromActor)
	{
		return FromActor->FindComponentByClass<USActionComponent>();
	}
	return nullptr;
}