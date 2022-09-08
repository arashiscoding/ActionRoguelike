// Fill out your copyright notice in the Description page of Project Settings.

#include "Widget/SWorldUserWidget.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if(!IsValid(AttachedActor))
	{
		RemoveFromParent();
		
		UE_LOG(LogTemp, Warning, TEXT("SWorldUserWidget | AttachedActor no longer valid, removing Health Widget."));
		return;
	}
	
	/**
	 * we could also use UGameplayStatics::ProjectWorldToScreen, but then we had to divide
	 * ScreenPosition by UWidgetLayoutLibrary::GetViewportScale to get the correct ScreenPosition for the widget
	 */
	FVector2D ScreenPosition{};
	if(UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition, false))
	{
		if(ParentSizeBox)
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
