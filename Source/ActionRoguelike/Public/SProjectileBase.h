// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SProjectileBase.generated.h"

class USphereComponent;
class UParticleSystemComponent;
class UProjectileMovementComponent;
class UParticleSystem;
 
UCLASS(Abstract)
class ACTIONROGUELIKE_API ASProjectileBase : public AActor
{
	GENERATED_BODY()
	
public:	
	ASProjectileBase();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComp{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent* EffectComp{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UProjectileMovementComponent* MovementComp{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Effects")
	UParticleSystem* ImpactVFX{};
	
protected:
	virtual void PostInitializeComponents() override;
	
	UFUNCTION()
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	UFUNCTION()
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Explode();
};
