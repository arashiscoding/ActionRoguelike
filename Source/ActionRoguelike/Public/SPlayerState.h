// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SPlayerState.generated.h"

class ASPlayerState;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCreditsChanged, ASPlayerState*, PlayerState, int32, NewCredits, int32, Delta);

UCLASS()
class ACTIONROGUELIKE_API ASPlayerState : public APlayerState
{
	GENERATED_BODY()

protected:
	UPROPERTY(ReplicatedUsing = "OnRep_Credits", EditDefaultsOnly, Category = "Credits");
	int32 Credits{};

	// OnRep can use a parameter containing the 'old value' of the variable it is bound to.
	UFUNCTION()
	void OnRep_Credits(int32 OldCredits);

public:
	UFUNCTION(BlueprintCallable, Category = "Credits")
	void AddCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	bool RemoveCredits(int32 Delta);

	UFUNCTION(BlueprintCallable, Category = "Credits")
	int32 GetCredits() const;
	
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCreditsChanged OnCreditChange{};
};
