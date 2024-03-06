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
	/** Sphere radius of the sweep to find desired target under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category= "Targeting")
	float SweepRadius{20.0f};
	
	/** Fallback distance when sweep finds no collision under crosshair. Adjusts final projectile direction */
	UPROPERTY(EditAnywhere, Category= "Targeting")
	float SweepDistance{5000.0f};
	
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
