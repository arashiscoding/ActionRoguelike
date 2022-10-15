// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"
#include "Net/UnrealNetwork.h"


void ASPlayerState::AddCredits(int32 Delta)
{
	int32 AbsoluteDelta = FMath::Abs(Delta);
	
	Credits += AbsoluteDelta;
	
	OnCreditChange.Broadcast(this, Credits, AbsoluteDelta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	int32 AbsoluteDelta = FMath::Abs(Delta);
	
	if(Credits < AbsoluteDelta)
	{
		return false;
	}

	Credits -= AbsoluteDelta;
	
	OnCreditChange.Broadcast(this, Credits, -AbsoluteDelta);
	
	return true;
}

int32 ASPlayerState::GetCredits() const
{
	return Credits;
}

void ASPlayerState::OnRep_Credits(int32 OldCredits)
{
	OnCreditChange.Broadcast(this, Credits, Credits - OldCredits);
}

void ASPlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPlayerState, Credits);
}