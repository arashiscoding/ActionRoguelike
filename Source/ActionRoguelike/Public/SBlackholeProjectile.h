// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SProjectileBase.h"
#include "SBlackholeProjectile.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ASBlackholeProjectile : public ASProjectileBase
{
	GENERATED_BODY()
	
public:	
	ASBlackholeProjectile();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	URadialForceComponent* RadialForceComp{};

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	void DestroyBlackhole();

	UFUNCTION()
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
private:
	FTimerHandle TimerHandle_BlackholeLifetime{};
};
