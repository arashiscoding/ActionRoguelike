// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class USAttributeComponent;
class USActionComponent;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	//Parameter's name in character's material
	UPROPERTY(VisibleDefaultsOnly, Category = "Effect|Material")
	FName HitReceivedTimeName{"HitReceivedTime"};

	//Parameter's name in character's material
	UPROPERTY(VisibleDefaultsOnly, Category = "Effect|Material")
	FName HitFlashSpeedName{"HitFlashSpeed"};

	//Parameter's name in character's material
	UPROPERTY(VisibleDefaultsOnly, Category = "Effect|Material")
	FName HitFlashColorName{"HitFlashColor"};

	// Material Vector Parameter value, represents RGB
	UPROPERTY(EditDefaultsOnly, Category = "Effect|Material")
	FVector HitFlashDamageColorValue{1.0f, 0.04f, 0.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Effect|Material")
	float HitFlashDamageSpeed{4.0f};

	// Material Vector Parameter value, represents RGB
	UPROPERTY(EditDefaultsOnly, Category = "Effect|Material")
	FVector HitFlashHealColorValue{0.0f, 0.8f, 0.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Effect|Material")
	float HitFlashHealSpeed{2.5f};
	
public:
	ASCharacter();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USInteractionComponent* InteractionComp{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp{};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USActionComponent* ActionComp{};
	
protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void SprintStart();
	void SprintStop();

	void PrimaryInteract();
	
	void PrimaryAttack();
	void SpawnBlackhole();
	void PerformDash();
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:
	FVector GetPawnViewLocation() const override;
	
private:
	/**
	 * Functions marked with Exec can be used as console commands
	 * Exec works only in few places: PlayerController, Playable Character, GameMode, CheatManager
	 */
	UFUNCTION(Exec)
	void HealSelf(float Amount = 100.0f);
};
