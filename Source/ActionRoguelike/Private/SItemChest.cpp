// Fill out your copyright notice in the Description page of Project Settings.

#include "SItemChest.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	SetRootComponent(BaseMesh);

	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	/* Member variables of this actor that are marked with "Replicated",
	 * will now be considered in Replication System */
	bReplicates = true;
}

void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	bIsLidOpen = !bIsLidOpen;

	/* In C++, RepNotifies are only triggered automatically for clients.
	 * So we have to call it manually here */
	OnRep_LidChanged();
}

void ASItemChest::OnRep_LidChanged()
{
	float CurrentPitch = bIsLidOpen ? TargetPitch : 0.0f;
	LidMesh->SetRelativeRotation(FRotator{CurrentPitch,0,0});
}

/* Setting up the replication rules here */
void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	/* means whenever "bIsLidOpen" has changed in the server, send it to all clients */
	DOREPLIFETIME(ASItemChest, bIsLidOpen);
}