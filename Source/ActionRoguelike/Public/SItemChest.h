// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	ASItemChest();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseMesh{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LidMesh{};

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Effect")
	float TargetPitch{110.0f};

	/* "ReplicatedUsing" let's us bind this variable to a particular function,
	 * so whenever this value changes in server, Unreal will call that function for us.
	 * Referred to as "RepNotify" in Blueprint */
	UPROPERTY(ReplicatedUsing = "OnRep_LidChanged", BlueprintReadOnly)
	bool bIsLidOpen{};

private:
	virtual void Interact_Implementation(APawn* InstigatorPawn) override;

	UFUNCTION()
	void OnRep_LidChanged();
};
