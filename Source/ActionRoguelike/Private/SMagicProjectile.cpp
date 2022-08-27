// Fill out your copyright notice in the Description page of Project Settings.

#include "SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "Kismet/GameplayStatics.h"


ASMagicProjectile::ASMagicProjectile()
{
	
}


void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASMagicProjectile::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor != GetInstigator())
	{
		USAttributeComponent* AttributeComponent = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		if(AttributeComponent)
		{
			AttributeComponent->ApplyHealthChange(-20.0f);

			Destroy();
		}
	}
}
