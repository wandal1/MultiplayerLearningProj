// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameMode.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Components/SHealthComponent.h"
#include "SGameState.h"
#include "SPlayerState.h"

ASGameMode::ASGameMode()
{
	GameStateClass = ASGameState::StaticClass();
	PlayerStateClass = ASPlayerState::StaticClass();

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
	PrimaryActorTick.TickInterval = 1;
}

void ASGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CheckWaveState();
	CheckAnyPlayerAlive();
	//RestartDeadPlayer();
}

void ASGameMode::StartWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Started"));

	CurrentWave++;

	BotToSpawnThisWave = CurrentWave * 2;

	GetWorldTimerManager().SetTimer(BotSpawner_TH, this, &ASGameMode::SpawnBotProcess, SpawnInterval, true, 0);

	SetWaveState(EWaveState::WaveInPropgress);
}

void ASGameMode::EndWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Ended"));

	GetWorldTimerManager().ClearTimer(BotSpawner_TH);

	SetWaveState(EWaveState::WaitingToComplete);
}

void ASGameMode::PrepareForNextWave()
{
	UE_LOG(LogTemp, Warning, TEXT("Wave Spawn Preparation"));

	GetWorldTimerManager().SetTimer(StartNextWave_TH, this, &ASGameMode::StartWave, TimeBetweenWaves, false);

	SetWaveState(EWaveState::WaitingToStart);
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

		SetWaveState(EWaveState::WaveComplete);
		PrepareForNextWave();
	}

}

void ASGameMode::CheckAnyPlayerAlive()
{
	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			USHealthComponent* HealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));

			if (ensure(HealthComp) && HealthComp->IsAlive())
			{
				return;
			}
		}
	}
	//If any player alive
	GameOver();
}

void ASGameMode::RestartDeadPlayer()
{

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && !PC->GetPawn())
		{
			RestartPlayer(PC);
		}
	}
}

void ASGameMode::GameOver()
{
	EndWave();

	UE_LOG(LogTemp, Error, TEXT("Game Over!! Player is Dead"));

	SetWaveState(EWaveState::GameOver);

	SetActorTickEnabled(false);
}

void ASGameMode::SetWaveState(EWaveState NewState)
{
	static ASGameState* GS = GetGameState<ASGameState>();

	//if (ensureAlways(GS))
	if (GS->IsValidLowLevel())
	{
		GS->SetWaveState(NewState);
	}
}

bool ASGameMode::TryResurectDeadPlayers()
{
	RestartDeadPlayer();
	return true;
}

void ASGameMode::StartPlay()
{
	Super::StartPlay();

	if (bAutoSpawn)
	{
		FTimerHandle Tmp_TH;
		GetWorldTimerManager().SetTimer(Tmp_TH, this, &ASGameMode::PrepareForNextWave, TimeBetweenWaves, false);

		SetActorTickEnabled(true);
	}
}
