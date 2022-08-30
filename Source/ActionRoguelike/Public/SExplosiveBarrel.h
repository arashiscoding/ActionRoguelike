// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

class URadialForceComponent;

UCLASS()
class ACTIONROGUELIKE_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	ASExplosiveBarrel();

protected:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* MeshComp{};

	UPROPERTY(EditDefaultsOnly)
	URadialForceComponent* RadialForceComp{};

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHitComponent(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
