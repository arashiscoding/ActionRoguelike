// Fill out your copyright notice in the Description page of Project Settings.

#include "Powerup/SPowerupActor.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"


ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	SetReplicates(true);
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes
}

void ASPowerupActor::DisablePowerup()
{
	bIsActive = false;
	
	// Reminder: When using c++, we have to explicitly call the OnRep functions because they don't get called for the server
	OnRep_IsActive();
	
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::EnablePowerup, RespawnTime);
}

void ASPowerupActor::EnablePowerup()
{
	bIsActive = true;
	OnRep_IsActive();
}

void ASPowerupActor::OnRep_IsActive()
{
	SetActorEnableCollision(bIsActive);
	RootComponent->SetVisibility(bIsActive, true);
}

void ASPowerupActor::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASPowerupActor, bIsActive);
}