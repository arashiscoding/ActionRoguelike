// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/SAction.h"
#include "SAction_ProjectileAttack.generated.h"

class ASProjectileBase;
class UParticleSystem;

UCLASS()
class ACTIONROGUELIKE_API USAction_ProjectileAttack : public USAction
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASProjectileBase> ProjectileClass{};

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	FName HandSocketName{"Muzzle_01"};

	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim{};

	UPROPERTY(EditAnywhere, Category = "Attack")
	float AttackAnimDelay{0.2f};
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UParticleSystem* CastingEffect{};

public:
	virtual void StartAction_Implementation(AActor* Instigator) override;

protected:
	UFUNCTION()
	void AttackDelay_Elapsed(ACharacter* InstigatorCharacter);

};
