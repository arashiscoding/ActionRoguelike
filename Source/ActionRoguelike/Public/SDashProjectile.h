// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SDashProjectile.generated.h"


UCLASS()
class ACTIONROGUELIKE_API ASDashProjectile : public ASProjectileBase
{
	GENERATED_BODY()

public:
	ASDashProjectile();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float TeleportDelay{0.2f};

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	float DetonateDelay{0.2f};

	FTimerHandle TimerHandle_Detonate{};

protected:
	virtual void BeginPlay() override;
	void Explode_Implementation() override;
	void TeleportInstigator();
};
