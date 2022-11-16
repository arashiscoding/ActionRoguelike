// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile/SDashProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	MovementComp->InitialSpeed = 6000.0f;
}

void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_Detonate, this, &ASDashProjectile::Explode, DetonateDelay);
}

void ASDashProjectile::Explode_Implementation()
{
	// Super::Explode_Implementation();
	
	/* in ProjectileBaseClass, we call Explode in OnActorHit and here, in BeginPLay, we set a timer to trigger Explode
	 * if we hit sth that's before the timer goes off, like floor, both OnActorHit and the timer will call Explode
	 * in order to prevent that, we clear the timer */
	GetWorldTimerManager().ClearTimer(TimerHandle_Detonate);

	UGameplayStatics::SpawnEmitterAtLocation(this, ImpactVFX, GetActorLocation(), GetActorRotation());

	EffectComp->DeactivateSystem();

	MovementComp->StopMovementImmediately();
	SetActorEnableCollision(false);

	FTimerHandle TimerHandle_Teleport{};
	GetWorldTimerManager().SetTimer(TimerHandle_Teleport, this, &ASDashProjectile::TeleportInstigator, TeleportDelay);
}

void ASDashProjectile::TeleportInstigator()
{
	AActor* MyInstigator = GetInstigator();
	if(ensure(MyInstigator))
	{
		MyInstigator->TeleportTo(GetActorLocation(), MyInstigator->GetActorRotation(), false, false);
	}
	Destroy();
}
