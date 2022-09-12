// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SAICharacter.h"
#include "AIController.h"
#include "BrainComponent.h"
#include "DrawDebugHelpers.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/PawnSensingComponent.h"
#include "Widget/SWorldUserWidget.h"


ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComp"));
	
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnSeePawn);

	AttributeComp->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnSeePawn(APawn* Pawn)
{
	SetTargetActor(Pawn);
	DrawDebugString(GetWorld(), GetActorLocation(), TEXT("Player Spotted!"), nullptr, FColor::Red, 4.0f, true);
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(Delta >= 0.0f)
	{
		return;
	}

	// If bot hasn't seen the player yet and player shoots at it, it can get aware of the player
	if(InstigatorActor != this)
	{
		SetTargetActor(InstigatorActor);
	}

	if(!HealthBarWidget)
	{
		HealthBarWidget = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
		if(HealthBarWidget)
		{
			HealthBarWidget->AttachedActor = this;

			//AddToViewport internally calls Construct. So we have to do any initialization before it.
			HealthBarWidget->AddToViewport();
		}
	}

	GetMesh()->SetScalarParameterValueOnMaterials(HitReceivedTimeName, GetWorld()->TimeSeconds);

	// Died
	if(!AttributeComp->IsAlive())
	{
		// stop BehaviorTree
		AAIController* MyAIController = Cast<AAIController>(GetController());
		if(MyAIController)
		{
			MyAIController->GetBrainComponent()->StopLogic("Killed");
		}

		// ragdoll
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetAllBodiesSimulatePhysics(true);

		// disable collision to avoid colliding with player and projectiles
		// disable CharacterMovement to avoid applying gravity when there's no collision
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCharacterMovement()->DisableMovement();

		// SetLifeSpan will destroy the actor after this amount of time(s)
		SetLifeSpan(10.0f);
	}
}

void ASAICharacter::SetTargetActor(AActor* TargetActor)
{
	AAIController* MyAIController = Cast<AAIController>(GetController());
	if(MyAIController)
	{
		MyAIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", TargetActor);
	}
}
