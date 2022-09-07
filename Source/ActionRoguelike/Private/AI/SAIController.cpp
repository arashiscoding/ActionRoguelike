// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAIController.h"
#include "Kismet/GameplayStatics.h"

void ASAIController::BeginPlay()
{
	Super::BeginPlay();

	if(ensureMsgf(BehaviorTree, TEXT("BehaviorTree is nullptr!")))
	{
		RunBehaviorTree(BehaviorTree);
	}
	
	// APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	// if(PlayerPawn)
	// {
	// 	GetBlackboardComponent()->SetValueAsObject("TargetActor", PlayerPawn);
	// }
}