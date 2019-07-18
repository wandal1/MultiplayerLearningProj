// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SGameMode.generated.h"

/**
 * 
 */
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

	FTimerHandle BotSpawner_TH;
	FTimerHandle StartNextWave_TH;

	UPROPERTY(EditDefaultsOnly)
		float SpawnInterval = 2;
	UPROPERTY(EditDefaultsOnly)
		float TimeBetweenWaves = 2;

	int BotToSpawnThisWave;

	int CurrentWave = 0;
public:
	virtual void StartPlay() override;

};
