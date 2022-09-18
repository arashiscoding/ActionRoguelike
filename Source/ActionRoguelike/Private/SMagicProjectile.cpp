// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "ActionSystem/SActionEffect.h"
#include "ActionSystem/SActionComponent.h"


ASMagicProjectile::ASMagicProjectile()
{
	AudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComp->SetupAttachment(RootComponent);
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	AudioComp->Activate();
}

void ASMagicProjectile::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		USActionComponent* ActionComp = OtherActor->FindComponentByClass<USActionComponent>();
		if(ActionComp && ActionComp->ActiveGameplayTags.HasTag(ParryTag))
		{
			/* in SProjectileBase, we set bRotationFollowsVelocity property of MovementComp to true,
			 *  so by inverting the Velocity, we will turn it around basically */
			MovementComp->Velocity = -MovementComp->Velocity;
			SetInstigator(Cast<APawn>(OtherActor));
			return;
		}
		
		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, DamageAmount, SweepResult))
		{
			UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, OtherActor->GetActorLocation(), FRotator::ZeroRotator);

			if(ActionComp)
			{
				ActionComp->AddAction(GetInstigator(), BurningActionClass);
			}
			
			Explode();
		}
	}
}

void ASMagicProjectile::OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, Hit.ImpactPoint, FRotator::ZeroRotator);
	AudioComp->Deactivate();
	
	Super::OnActorHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}
