// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"


USInteractionComponent::USInteractionComponent()
{
	
}

void USInteractionComponent::PrimaryInteract()
{
	ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());

	if (!ensure(MyOwner))
	{
		return;
	}

	FHitResult HitResult{};

	FVector TraceStart{MyOwner->GetCameraLocation() + (MyOwner->GetControlRotation().Vector() * 30.0f)};
	FVector TraceEnd{TraceStart + (MyOwner->GetControlRotation().Vector() * 1000.0f)};

	FCollisionObjectQueryParams ObjectQueryParams{};
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);


	FCollisionShape CollisionShape;
	float ShapeRadius{30.0f};
	CollisionShape.SetSphere(ShapeRadius);

	//GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape);

	bool DidItHit = GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape);
	FColor DebugShapeColor = DidItHit ? FColor::Green : FColor::Red;

	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->Implements<USGameplayInterface>()) //whenever we want to check or cast, we use U
		{
			ISGameplayInterface::Execute_Interact(HitActor, MyOwner); //whenever we want to call it, we use I
			DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, ShapeRadius, 16, DebugShapeColor, false, 2.0f);
		}
	}
	DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugShapeColor, false, 2.0f, 0, 2);
}
