// Fill out your copyright notice in the Description page of Project Settings.

#include "SInteractionComponent.h"
#include "DrawDebugHelpers.h"
#include "SCharacter.h"
#include "SGameplayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Widget/SWorldUserWidget.h"

static TAutoConsoleVariable<bool> CVarDrawDebugInteractions(TEXT("ara.DrawDebugInteractions"), false, TEXT("Enable debug lines for Interaction component"), ECVF_Cheat);

USInteractionComponent::USInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FindBestInteractable();
}

void USInteractionComponent::FindBestInteractable()
{
	ASCharacter* MyOwner = Cast<ASCharacter>(GetOwner());

	if (!ensure(MyOwner))
	{
		return;
	}

	FHitResult HitResult{};

	FVector TraceStart{MyOwner->GetPawnViewLocation() + (MyOwner->GetControlRotation().Vector() * 30.0f)};
	FVector TraceEnd{TraceStart + (MyOwner->GetControlRotation().Vector() * TraceDistance)};

	FCollisionObjectQueryParams ObjectQueryParams{};
	ObjectQueryParams.AddObjectTypesToQuery(TraceCollisionChannel);


	FCollisionShape CollisionShape;
	CollisionShape.SetSphere(TraceRadius);


	bool DidItHit = GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape);

	FColor DebugShapeColor = DidItHit ? FColor::Green : FColor::Red;

	bool bDrawDebug = CVarDrawDebugInteractions.GetValueOnGameThread();

	FocusedActor = nullptr;

	if (AActor* HitActor = HitResult.GetActor())
	{
		if (HitActor->Implements<USGameplayInterface>()) //whenever we want to check or cast to an Interface, we use U
		{
			FocusedActor = HitActor;
			if (bDrawDebug)
			{
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, TraceRadius, 16, DebugShapeColor, false, 2.0f);
			}
		}
	}
	
	if (bDrawDebug)
	{
		DrawDebugLine(GetWorld(), TraceStart, TraceEnd, DebugShapeColor, false, 2.0f, 0, 2);
	}

	if(FocusedActor)
	{
		if(!PopupWidget && ensure(PopupWidgetClass))
		{
			PopupWidget = CreateWidget<USWorldUserWidget>(GetWorld(), PopupWidgetClass);
		}
		
		if(PopupWidget)
		{
			PopupWidget->AttachedActor = FocusedActor;
			
			if(!PopupWidget->IsInViewport())
			{
				PopupWidget->AddToViewport();
			}
		}
	}
	else
	{
		if(PopupWidget)
		{
			PopupWidget->RemoveFromParent();
		}
	}
}

void USInteractionComponent::PrimaryInteract()
{
	if(!FocusedActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("No FocusedActor to interact."));
		return;
	}
	
	APawn* MyPawn = Cast<APawn>(GetOwner());
	ISGameplayInterface::Execute_Interact(FocusedActor, MyPawn); //whenever we want to call an Interface, we use I
}
