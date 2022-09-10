// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"
#include "AIController.h"
#include "SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	if(!ensureMsgf(OwnerComp.GetBlackboardComponent(), TEXT("Add Blackborad Component")))
	{
		return;
	}

	APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(ensure(AIPawn))
	{
		USAttributeComponent* AttributeComp = USAttributeComponent::GetAttributeComp(AIPawn);
		if(ensure(AttributeComp))
		{
			bool bIsLowOnHealth = (AttributeComp->GetHealth() / AttributeComp->GetMaxHealth()) < LowHealthThreshold;
			
			UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, bIsLowOnHealth);
		}
	}
}
