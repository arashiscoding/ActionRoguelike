// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;

UCLASS()
class ACTIONROGUELIKE_API ASAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASAICharacter();

protected:
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UPawnSensingComponent* PawnSensingComp{};

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USAttributeComponent* AttributeComp{};

protected:
	//Parameter's name in character's material
	UPROPERTY(VisibleDefaultsOnly, Category = "Effect|Material")
	FName HitReceivedTimeName{"HitReceivedTime"};

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

	void SetTargetActor(AActor* TargetActor);
};
