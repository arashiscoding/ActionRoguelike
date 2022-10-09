// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"


static void LogOnScreen(UObject* WorldContextObject, FString Msg, FColor Color = FColor::White, float Duration = 5.0f)
{
	if(!ensure(WorldContextObject))
	{
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if(!ensure(World))
	{
		return;
	}

	FString NetPrefix = World->IsNetMode(NM_Client) ? "[CLIENT] " : "[SERVER] ";
	if(GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, Duration, Color, NetPrefix + Msg);
	}
}