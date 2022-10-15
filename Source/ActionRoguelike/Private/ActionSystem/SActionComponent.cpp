// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/SActionComponent.h"
#include "ActionRoguelike/ActionRoguelike.h"
#include "ActionSystem/SAction.h"
#include "Engine/ActorChannel.h"
#include "Net/UnrealNetwork.h"

USActionComponent::USActionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	
	SetIsReplicatedByDefault(true);
}


void USActionComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		for(TSubclassOf<USAction>& ActionClass : DefaultActions)
		{
			AddAction(GetOwner(), ActionClass);
		}
	}
}

void USActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// FString DebugMsg = GetNameSafe(GetOwner()) + " : " + ActiveGameplayTags.ToStringSimple();
	// GEngine->AddOnScreenDebugMessage(-1, 0.0f, FColor::White, DebugMsg);

	// for(USAction* Action : Actions)
	// {
	// 	FColor TextColor = Action->IsRunning() ? FColor::Blue : FColor::White;
	// 	
	// 	FString ActionMsg = FString::Printf(TEXT("[%s] | Action: %s"), *GetNameSafe(GetOwner()), *GetNameSafe(Action));
	// 	
	// 	LogOnScreen(this, ActionMsg, TextColor, 0.0f);
	// }
}

void USActionComponent::AddAction(AActor* Instigator, const TSubclassOf<USAction>& ActionClass)
{
	if(!ensure(ActionClass))
	{
		return;
	}

	// Skip for clients
	if(!GetOwner()->HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("Client attempting to AddAction: %s"), *GetNameSafe(ActionClass));
		return;
	}

	USAction* NewAction = NewObject<USAction>(GetOwner(), ActionClass);
	if(ensure(NewAction))
	{
		NewAction->Initialize(this);
		
		Actions.Add(NewAction);
		
		if(NewAction->bAutoStart)
		{
			if(NewAction->CanStart(Instigator))
			{
				StartActionByName(Instigator, NewAction->ActionName);
			}
			else
			{
				RemoveAction(NewAction);
			}
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
				// Is client?
				if(!GetOwner()->HasAuthority())
				{
					ServerStopAction(Instigator, ActionName);
				}
				
				Action->StopAction(Instigator);
				return true;
			}
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Action [%s] wasn't found. Unable to stop!"), *ActionName.ToString());
	return false;
}

USActionComponent* USActionComponent::GetActionComp(AActor* FromActor)
{
	if(FromActor)
	{
		return FromActor->FindComponentByClass<USActionComponent>();
	}
	return nullptr;
}

void USActionComponent::ServerStartAction_Implementation(AActor* Instigator, FName ActionName)
{
	StartActionByName(Instigator, ActionName);
}

void USActionComponent::ServerStopAction_Implementation(AActor* Instigator, FName ActionName)
{
	StopActionByName(Instigator, ActionName);
}

bool USActionComponent::ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags)
{
	bool WroteSomething = Super::ReplicateSubobjects(Channel, Bunch, RepFlags);
	for(USAction* Action : Actions)
	{
		if(Action)
		{
			// "Channel->ReplicateSubobject" checks if the variable has changed
			WroteSomething |= Channel->ReplicateSubobject(Action, *Bunch, *RepFlags);
		}
	}
	
	return WroteSomething;
}

void USActionComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(USActionComponent, Actions);
}