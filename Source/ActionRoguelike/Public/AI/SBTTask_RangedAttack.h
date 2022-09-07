// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "SBTTask_RangedAttack.generated.h"

class ASMagicProjectile;

UCLASS()
class ACTIONROGUELIKE_API USBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "AI")
	FBlackboardKeySelector TargetActorObject{};
	
	UPROPERTY(EditAnywhere, Category = "AI")
	TSubclassOf<ASMagicProjectile> MagicProjectileClass{};

	// maximum bullet spread for pitch and yaw in degree
	UPROPERTY(EditAnywhere, Category = "AI")
	float MaxBulletSpread{2.0f};
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
