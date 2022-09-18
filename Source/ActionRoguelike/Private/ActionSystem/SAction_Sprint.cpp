// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/SAction_Sprint.h"
#include "GameFramework/CharacterMovementComponent.h"


void USAction_Sprint::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UCharacterMovementComponent* CharacterMovementComp = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	if(CharacterMovementComp)
	{
		CharacterMovementComp->MaxWalkSpeed += BonusSpeed;
	}
}

void USAction_Sprint::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UCharacterMovementComponent* CharacterMovementComp = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	if(CharacterMovementComp)
	{
		CharacterMovementComp->MaxWalkSpeed -= BonusSpeed;
	}
}