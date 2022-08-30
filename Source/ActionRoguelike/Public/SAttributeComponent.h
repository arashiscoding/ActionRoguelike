// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SAttributeComponent.generated.h"

class USAttributeComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnHealthChanged, AActor*, InstigatorActor, USAttributeComponent*, OwningComp, float, NewHealth, float, Delta);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	USAttributeComponent();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attributes")
	float MaxHealth{100.0f};
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes")
	float Health{};

public:
	virtual void BeginPlay() override;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged{};
	
	UFUNCTION(BlueprintCallable)
	bool ApplyHealthChange(float Delta);

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	bool IsDamaged() const;

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const;
};
