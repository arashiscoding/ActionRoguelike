// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"

class USWorldUserWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ACTIONROGUELIKE_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	USInteractionComponent();

protected:
	UPROPERTY()
	AActor* FocusedActor{};

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceDistance{500.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	float TraceRadius{30.0f};

	UPROPERTY(EditDefaultsOnly, Category = "Trace")
	TEnumAsByte<ECollisionChannel> TraceCollisionChannel{ECC_WorldDynamic};

	
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USWorldUserWidget> PopupWidgetClass{};

	UPROPERTY()
	USWorldUserWidget* PopupWidget{};

protected:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void FindBestInteractable();

	/* Reliable - Will always arrive, eventually. Request will be re-sent unless an acknowledgment was received (TCP)
	 * Unreliable - Not guaranteed, packet can get lost and won't retry (UDP) */
	UFUNCTION(Server, Reliable)
	void ServerInteract(AActor* InFocus);
	
public:
	void PrimaryInteract();
	
};
