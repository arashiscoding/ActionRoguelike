// Fill out your copyright notice in the Description page of Project Settings.

#include "SBlackholeProjectile.h"

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBlackholeProjectile::ASBlackholeProjectile()
{
	RadialForceComp = CreateDefaultSubobject<URadialForceComponent>(TEXT("RadialForceComp"));
	RadialForceComp->SetupAttachment(RootComponent);
	RadialForceComp->Radius = 1000.0f;
	RadialForceComp->ForceStrength = -200'000.0f;

	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
}

void ASBlackholeProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASBlackholeProjectile::OnActorBeginOverlap);
}

void ASBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(TimerHandle_BlackholeLifetime, this, &ASBlackholeProjectile::DestroyBlackhole, 5.0f);
	AudioComp->Activate();
}

void ASBlackholeProjectile::DestroyBlackhole()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("BP_BlackholeProjectile | Blackhole destroyed!")));
	AudioComp->Deactivate();
	Destroy();
}

void ASBlackholeProjectile::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherComp->IsSimulatingPhysics())
	{
		OtherActor->Destroy();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("BLACKED!")));
	}
}
