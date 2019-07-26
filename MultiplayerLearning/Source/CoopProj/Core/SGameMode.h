// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

enum class EWaveState : uint8;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnActorKilled, AActor*, VictimActor, AActor*, KillerActor, AController*, KillerController);

UCLASS()
class COOPPROJ_API ASGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASGameMode();
protected:
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void SpawnNewBot();

	void StartWave();

	void EndWave();

	void PrepareForNextWave();

	void SpawnBotProcess();

	void CheckWaveState();

	void CheckAnyPlayerAlive();

	void RestartDeadPlayer();

	void GameOver();

	void SetWaveState(EWaveState NewState);

	FTimerHandle BotSpawner_TH;
	FTimerHandle StartNextWave_TH;
	UPROPERTY(EditDefaultsOnly)
		bool bAutoSpawn = true;
	UPROPERTY(EditDefaultsOnly)
		float SpawnInterval = 2;
	UPROPERTY(EditDefaultsOnly)
		float TimeBetweenWaves = 2;

	int BotToSpawnThisWave;

	int CurrentWave = 0;
public:
	bool TryResurectDeadPlayers();

	virtual void StartPlay() override;

	UPROPERTY(BlueprintAssignable)
		FOnActorKilled OnActorKilled;

};
