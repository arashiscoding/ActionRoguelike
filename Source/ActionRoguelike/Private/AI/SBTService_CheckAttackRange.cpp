// Fill out your copyright notice in the Description page of Project Settings.

#include "AI/SBTService_CheckAttackRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	//check distance between AI pawn and TargetActor

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(ensure(BlackboardComp))
	{
		AActor* TargetActor = Cast<AActor>(BlackboardComp->GetValueAsObject("TargetActor"));
		if(TargetActor)
		{
			AAIController* MyAIController = OwnerComp.GetAIOwner();
			if(ensure(MyAIController))
			{
				APawn* AIPawn = MyAIController->GetPawn();
				if(ensure(AIPawn))
				{
					float DistanceTo = (AIPawn->GetActorLocation() - TargetActor->GetActorLocation()).Size();
					bool bWithinRange = DistanceTo < AttackRangeDistance;
					
					bool bHasLOS{};
					if(bWithinRange)
					{
						bHasLOS = MyAIController->LineOfSightTo(TargetActor);
					}
					
					BlackboardComp->SetValueAsBool(AttackRangeKey.SelectedKeyName, (bWithinRange && bHasLOS));
				}
			}
		}
	}
}
 