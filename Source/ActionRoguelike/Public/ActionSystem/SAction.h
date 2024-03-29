// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

class USActionComponent;

USTRUCT()
struct FActionRepData
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	bool bIsRunning{};

	UPROPERTY()
	AActor* Instigator{};
};

UCLASS(Blueprintable)
class ACTIONROGUELIKE_API USAction : public UObject
{
	GENERATED_BODY()

public:
	/** Action name to start/stop without a reference to the object */
	UPROPERTY(EditDefaultsOnly, Category = "Action")
	FName ActionName{};

	UPROPERTY(EditDefaultsOnly, Category = "Action")
	bool bAutoStart{};

protected:
	UPROPERTY(Replicated)
	USActionComponent* ActionComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI")
	TSoftObjectPtr<UTexture2D> Icon{};
	
	/** Tags added to OwningActor when activated, removed when action stops */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer GrantsTags{};

	/** Action can only start if OwningActor has none of these tags */
	UPROPERTY(EditDefaultsOnly, Category = "Tags")
	FGameplayTagContainer BlockedTags{};

	UPROPERTY(Replicated)
	float TimeStarted{};

public:
	void Initialize(USActionComponent* NewActionComp);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	bool CanStart(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, Category = "Action")
	void StartAction(AActor* Instigator);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Action")
	void StopAction(AActor* Instigator);
	
	/** if we don't override this, we don't have access to its functions in Blueprint*/
	virtual UWorld* GetWorld() const override;

	UFUNCTION(BlueprintCallable, Category = "Action")
	bool IsRunning() const;

protected:
	UFUNCTION(BlueprintCallable, Category = "Action")
	USActionComponent* GetOwningComponent() const;

	UPROPERTY(ReplicatedUsing="OnRep_RepData");
	FActionRepData RepData{};

	UFUNCTION()
	void OnRep_RepData();

	/** Extra step for networking classes that derive from UObject */
	virtual bool IsSupportedForNetworking() const override
	{
		return true;
	}
};
