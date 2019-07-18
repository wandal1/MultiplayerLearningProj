// Fill out your copyright notice in the Description page of Project Settings.


#include "PowerUpActor.h"
#include "UnrealNetwork.h"

// Sets default values
APowerUpActor::APowerUpActor()
{
	SetReplicates(true);

	bIsPowerupActive = false;
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

		bIsPowerupActive = true;
		OnRep_PowerupActive();

		GetWorldTimerManager().ClearTimer(PowerupTick_TH);
	}

}

void APowerUpActor::OnRep_PowerupActive()
{
	OnPowerUpStateChanged(bIsPowerupActive);
}

void APowerUpActor::ActivatePowerUp(AActor* OtherActor)
{
	OnActivated(OtherActor);

	bIsPowerupActive = true;
	OnRep_PowerupActive();

	if (PowerUpsInterval > 0)
	{
		GetWorld()->GetTimerManager().SetTimer(PowerupTick_TH, this, &APowerUpActor::PowerUpTick, PowerUpsInterval, true);
	}
	else
	{
		PowerUpTick();
	}
}

void APowerUpActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APowerUpActor, bIsPowerupActive);
}

