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
	UPROPERTY(VisibleDefaultsOnly, Category = "Attack")
	FName HandSocketName{"Muzzle_01"};
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	UAnimMontage* AttackAnim{};
	
	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASMagicProjectile> MagicProjectileClass{};

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASBlackholeProjectile> BlackholeProjectileClass{};

	UPROPERTY(EditAnywhere, Category = "Attack")
	TSubclassOf<ASDashProjectile> DashProjectileClass{};


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

	
private:
	FTimerHandle TimerHandle_PrimaryAttack{};
	FTimerHandle TimerHandle_Blackhole{};
	FTimerHandle TimerHandle_Dash{};
	
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
	
protected:
	virtual void PostInitializeComponents() override;
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
	
	UFUNCTION()
	void OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta);

public:
	FVector GetCameraLocation() const;
};
