// Fill out your copyright notice in the Description page of Project Settings.


#include "Actions/SAction_ProjectileAttack.h"
#include "SProjectileBase.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


void USAction_ProjectileAttack::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	ACharacter* Character = Cast<ACharacter>(Instigator);
	if(Character)
	{
		Character->PlayAnimMontage(AttackAnim);

		UGameplayStatics::SpawnEmitterAttached(CastingEffect, Character->GetMesh(), HandSocketName, FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget);

		FTimerDelegate TimerDelegate{};
		TimerDelegate.BindUFunction(this, "AttackDelay_Elapsed", Character);

		FTimerHandle TimerHandle_AttackDelay{};
		GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDelegate, AttackAnimDelay, false);
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if(ensureAlways(ProjectileClass))
	{
		FCollisionShape CollisionShape{};
		CollisionShape.SetSphere(20.0f);

		FCollisionQueryParams CollisionQueryParams{};
		CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);

		FCollisionObjectQueryParams ObjectQueryParams{};
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

		FVector TraceStart{(InstigatorCharacter->GetPawnViewLocation()) + ((InstigatorCharacter->GetControlRotation().Vector() * 30))};
		FVector TraceEnd{TraceStart + (InstigatorCharacter->GetControlRotation().Vector() * 5000)};
		
		FHitResult HitResult{};
		if(GetWorld()->SweepSingleByObjectType(HitResult, TraceStart, TraceEnd, FQuat::Identity, ObjectQueryParams, CollisionShape, CollisionQueryParams))
		{
			TraceEnd = HitResult.ImpactPoint;
		}
		
		FVector HandLocation{InstigatorCharacter->GetMesh()->GetSocketLocation(HandSocketName)};
		
		FVector Direction = TraceEnd - HandLocation;
		FRotator ProjectileRotation = Direction.Rotation();
		
		//FRotator ProjectileRotation{UKismetMathLibrary::FindLookAtRotation(HandLocation, TraceEnd)};
		FTransform SpawnTM{ProjectileRotation, HandLocation};
		
		FActorSpawnParameters SpawnParameters{};
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = InstigatorCharacter;

		GetWorld()->SpawnActor<ASProjectileBase>(ProjectileClass, SpawnTM, SpawnParameters);
	}

	StopAction(InstigatorCharacter);
}