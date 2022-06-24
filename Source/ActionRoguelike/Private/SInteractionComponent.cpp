// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "DrawDebugHelpers.h"
#include "SGameplayInterface.h"


// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FHitResult Hit{};
	
	FCollisionObjectQueryParams ObjectQueryParams{};
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);

	APawn* MyOwner = Cast<APawn>(GetOwner());
	FVector EyeLocation{};
	FRotator EyeRotation{};
	MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

	FVector EndLocation = EyeLocation + (EyeRotation.Vector() * 1000.0f);
	
	// GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, EndLocation, ObjectQueryParams);
	

	float ShapeRadius{30.0f};
	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(ShapeRadius);
	
	bool DidWeBlock = GetWorld()->SweepSingleByObjectType(Hit, EyeLocation, EndLocation, FQuat::Identity, ObjectQueryParams, CollisionShape);
	FColor DebugShapeColor = DidWeBlock ? FColor::Green : FColor::Red;
	
	if(AActor* HitActor = Hit.GetActor())
	{
		if(HitActor->Implements<USGameplayInterface>()) //whenever we want to check or cast, we use U
		{
			ISGameplayInterface::Execute_Interact(HitActor, MyOwner); //whenever we want to call it, we use I
			DrawDebugSphere(GetWorld(), Hit.ImpactPoint, ShapeRadius, 16, DebugShapeColor, false, 2.0f);
		}
	}
	DrawDebugLine(GetWorld(), EyeLocation, EndLocation, DebugShapeColor, false, 2.0f, 0, 2);
}