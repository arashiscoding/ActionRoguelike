// Fill out your copyright notice in the Description page of Project Settings.

#include "SItemChest.h"
#include "ActionSystem/SActionComponent.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	if(!bKeyRequired)
	{
		ChangeLidState();
		return;
	}

	USActionComponent* ActionComp = USActionComponent::GetActionComp(InstigatorPawn);
	if(ensure(ActionComp))
	{
		if(ActionComp->ActiveGameplayTags.HasTag(RequiredKeyCard))
		{
			ChangeLidState();
		}
	}
}

void ASItemChest::ChangeLidState()
{
	bLidOpened = !bLidOpened;

	// In C++, RepNotifies are only triggered automatically for clients.
	// So we have to call it manually for server
	OnRep_LidOpened();
}

void ASItemChest::OnRep_LidOpened()
{
	AnimateToggleLid();
}

void ASItemChest::OnActorLoaded_Implementation()
{
	float CurrentPitch = bLidOpened ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator{CurrentPitch,0,0});
}

FText ASItemChest::GetInteractText_Implementation(APawn* InstigatorPawn)
{
	USActionComponent* ActionComp = USActionComponent::GetActionComp(InstigatorPawn);
	if(ensure(ActionComp))
	{
		if(ActionComp->ActiveGameplayTags.HasTag(RequiredKeyCard))
		{
			return FText::GetEmpty();
		}
		else
		{
			return NSLOCTEXT("InteractableActors", "ItemChest_KeyRequired", "Corresponding key required!");
		}
	}
	
	return FText::GetEmpty();
}

/* Setting up the replication rules here */
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* means whenever these values have changed in the server, send them to all clients */
	DOREPLIFETIME(ASItemChest, bLidOpened);
}