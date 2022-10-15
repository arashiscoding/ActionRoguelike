// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SPowerupActor.generated.h"

class USphereComponent;

UCLASS()
class ACTIONROGUELIKE_API ASPowerupActor : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:
	ASPowerupActor();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp{};
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MeshComp{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	float RespawnTime{10.0f};

	UPROPERTY(ReplicatedUsing = "OnRep_IsActive")
	bool bIsActive{true};
	
protected:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	void DisablePowerup();
	void EnablePowerup();

	UFUNCTION()
	void OnRep_IsActive();

private:
	FTimerHandle TimerHandle_RespawnTimer;
};
