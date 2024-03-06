// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SAICharacter.generated.h"

class UPawnSensingComponent;
class USAttributeComponent;
class USActionComponent;
class USoundCue;
class USWorldUserWidget;

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

	UPROPERTY(VisibleAnywhere, Category = "Components")
	USActionComponent* ActionComp{};

protected:
	/* Parameter's name in character's material */
	UPROPERTY(VisibleDefaultsOnly, Category = "Effect")
	FName HitReceivedTimeName{"HitReceivedTime"};

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> HealthBarWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> SpottedWidgetClass{};
	
	UPROPERTY(EditDefaultsOnly, Category = "AI", meta = (ClampMin = "2.0", UIMin = "2.0"))
	float TimeToForgetPlayer{5.0f};

protected:
	virtual void PostInitializeComponents() override;

	UFUNCTION()
	void OnSeePawn(APawn* Pawn);

	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);
	
	void SetTargetActor(AActor* TargetActor);
	AActor* GetTargetActor() const;

	UFUNCTION()
	void ForgetPlayer();

	void ShowSpottedWidget();

	UPROPERTY()
	USWorldUserWidget* HealthBarWidget{};

	UPROPERTY()
	USWorldUserWidget* SpottedWidget{};

	FTimerHandle TimerHandle_ForgetPlayer{};
};