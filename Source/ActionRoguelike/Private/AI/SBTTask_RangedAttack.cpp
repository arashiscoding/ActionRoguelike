// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTTask_RangedAttack.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "SMagicProjectile.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIController = OwnerComp.GetAIOwner();
	if(ensure(AIController))
	{
		ACharacter* MyAICharacter = AIController->GetCharacter();
		if(!MyAICharacter)
		{
			return EBTNodeResult::Failed;
		}
		
		AActor* TargetActor = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetActorObject.SelectedKeyName));
		if(!TargetActor)
		{
			return EBTNodeResult::Failed;
		}

		if(!USAttributeComponent::IsActorAlive(TargetActor))
		{
			return EBTNodeResult::Failed;
		}
		
		FVector MuzzleLocation = MyAICharacter->GetMesh()->GetSocketLocation("Muzzle_01");
		
		//exactly what UKismetMathLibrary::FindLookAtRotation is
		FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		FRotator MuzzleRotation = Direction.Rotation();

		MuzzleRotation.Pitch += FMath::RandRange(0.0f, MaxBulletSpread);
		MuzzleRotation.Yaw += FMath::RandRange(-MaxBulletSpread, MaxBulletSpread);
		
		FActorSpawnParameters SpawnParameters{};
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParameters.Instigator = MyAICharacter;

		AActor* NewProjectile = GetWorld()->SpawnActor<ASMagicProjectile>(MagicProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParameters);
		return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
