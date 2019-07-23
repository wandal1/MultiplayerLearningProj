// Fill out your copyright notice in the Description page of Project Settings.


#include "SResurectionSpot.h"
#include "Core/SGameMode.h"

void ASResurectionSpot::TimerFinished()
{
	ASGameMode* GM = Cast<ASGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->TryResurectDeadPlayers();
	}
}
