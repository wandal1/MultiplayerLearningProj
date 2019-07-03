// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpActor.h"

// Sets default values
APowerUpActor::APowerUpActor()
{

}

// Called when the game starts or when spawned
void APowerUpActor::BeginPlay()
{
	Super::BeginPlay();

}

void APowerUpActor::PowerUpTick()
{
	TickMade++;

	OnPowerUpTicked();

	if (TickMade >= TotalNOfTicks )
	{
		OnExpired();

		GetWorldTimerManager().ClearTimer(PowerupTick_TH);
	}

}

void APowerUpActor::ActivatePowerUp()
{
	OnActivated();

	if (PowerUpsInterval > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(PowerupTick_TH, this, &APowerUpActor::PowerUpTick, PowerUpsInterval, true);
	}
	else
	{
		PowerUpTick();
	}
}

