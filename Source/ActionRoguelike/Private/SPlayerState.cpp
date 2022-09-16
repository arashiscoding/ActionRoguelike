// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

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
