// Fill out your copyright notice in the Description page of Project Settings.


#include "ActionSystem/SAction_ProjectileAttack.h"
#include "Projectile/SProjectileBase.h"
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

		if(Character->HasAuthority())
		{
			FTimerHandle TimerHandle_AttackDelay{};
			FTimerDelegate TimerDelegate{};
			TimerDelegate.BindUFunction(this, "AttackDelay_Elapsed", Character);
			
			GetWorld()->GetTimerManager().SetTimer(TimerHandle_AttackDelay, TimerDelegate, AttackAnimDelay, false);
		}
	}
}

void USAction_ProjectileAttack::AttackDelay_Elapsed(ACharacter* InstigatorCharacter)
{
	if(!ensureAlways(ProjectileClass))
	{
		StopAction(InstigatorCharacter);
		return;
	}
	
	FCollisionShape CollisionShape{};
	CollisionShape.SetSphere(SweepRadius);

	// Ignore Player
	FCollisionQueryParams CollisionQueryParams{};
	CollisionQueryParams.AddIgnoredActor(InstigatorCharacter);

	FVector TraceDirection = InstigatorCharacter->GetControlRotation().Vector();
	
	// Add sweep radius onto start to avoid the sphere clipping into floor/walls the camera is directly against
	FVector TraceStart = InstigatorCharacter->GetPawnViewLocation() + (TraceDirection * SweepRadius);
	FVector TraceEnd = TraceStart + (TraceDirection * SweepDistance);
	
	FHitResult HitResult{};
	if(GetWorld()->SweepSingleByChannel(HitResult, TraceStart, TraceEnd, FQuat::Identity, ECC_GameTraceChannel1, CollisionShape, CollisionQueryParams))
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

	StopAction(InstigatorCharacter);
}