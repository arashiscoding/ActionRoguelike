// Fill out your copyright notice in the Description page of Project Settings.

#include "SPowerupActor.h"
#include "Components/SphereComponent.h"


ASPowerupActor::ASPowerupActor()
{
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SetRootComponent(SphereComp);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ASPowerupActor::Interact_Implementation(APawn* InstigatorPawn)
{
	// logic in derived classes
}

void ASPowerupActor::DisablePowerup()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	GetWorldTimerManager().SetTimer(TimerHandle_RespawnTimer, this, &ASPowerupActor::EnablePowerup, RespawnTime);
}

void ASPowerupActor::EnablePowerup()
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
}