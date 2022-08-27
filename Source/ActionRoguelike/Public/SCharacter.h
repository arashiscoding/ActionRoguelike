// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class USAttributeComponent;

class ASProjectileBase;
class ASMagicProjectile;
class ASBlackholeProjectile;
class ASDashProjectile;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim{};
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASMagicProjectile> MagicProjectileClass{};

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASBlackholeProjectile> BlackholeProjectileClass{};

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASDashProjectile> DashProjectileClass{};

private:
	FTimerHandle TimerHandle_PrimaryAttack{};
	FTimerHandle TimerHandle_Blackhole{};
	FTimerHandle TimerHandle_Dash{};
	
public:
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USInteractionComponent* InteractionComp{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	USAttributeComponent* AttributeComp{};
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryInteract();
	
	void PrimaryAttack();
	void PrimaryAttack_TimeElapsed();

	void SpawnBlackhole();
	void SpawnBlackhole_TimeElapsed();

	void PerformDash();
	void PerformDash_TimeElapsed();
	
	void SpawnProjectile(TSubclassOf<ASProjectileBase> ClassToSpawn);
};
