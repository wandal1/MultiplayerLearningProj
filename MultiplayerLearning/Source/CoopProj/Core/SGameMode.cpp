// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/SHealthComponent.h"

ASGameMode::ASGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 1;
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
}

void ASGameMode::StartWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Started"));

	CurrentWave++;

	BotToSpawnThisWave = CurrentWave * 2;

	GetWorldTimerManager().SetTimer(BotSpawner_TH, this, &ASGameMode::SpawnBotProcess, SpawnInterval, true, 0);
}

void ASGameMode::EndWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Ended"));

	GetWorldTimerManager().ClearTimer(BotSpawner_TH);
}

void ASGameMode::PrepareForNextWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Preparation"));

	GetWorldTimerManager().SetTimer(StartNextWave_TH, this, &ASGameMode::StartWave, TimeBetweenWaves, false);
}

void ASGameMode::SpawnBotProcess()
{

	UE_LOG(LogTemp, Warning, TEXT("Spawn Bot Process"));

	SpawnNewBot();

	BotToSpawnThisWave--;

	if (BotToSpawnThisWave <= 0)
	{
		EndWave();
	}
}

void ASGameMode::CheckWaveState()
{
	if (BotToSpawnThisWave > 0 || GetWorldTimerManager().IsTimerActive(StartNextWave_TH))
	{
		return;
	}

	bool bIsAnyBotAlive = false;

	for (FConstPawnIterator It = GetWorld()->GetPawnIterator(); It; ++It)
	{
		APawn* TmpPawn = It->Get();
		if (TmpPawn && TmpPawn->IsPlayerControlled())
		{
			continue;
		}

		USHealthComponent* HP_Comp = Cast<USHealthComponent>(TmpPawn->GetComponentByClass(USHealthComponent::StaticClass()));

		if (HP_Comp && HP_Comp->IsAlive())
		{
			bIsAnyBotAlive = true;
			break;
		}
	}

	if (!bIsAnyBotAlive)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Bots Left Alive"));
		PrepareForNextWave();
	}

}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	PrepareForNextWave();
}
