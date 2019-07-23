// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "SGameState.generated.h"

UENUM(BlueprintType)
enum class EWaveState : uint8
{
	WaitingToStart,
	WaveInPropgress,
	/*Waiting until all bots will be killed*/
	WaitingToComplete,
	WaveComplete,
	GameOver
};

/**
 * 
 */
UCLASS()
class COOPPROJ_API ASGameState : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	void SetWaveState(EWaveState NewState);

protected:
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnRep_WaveState)
	EWaveState WaveState;

	UFUNCTION()
	void OnRep_WaveState(EWaveState OldState);

	UFUNCTION(BlueprintImplementableEvent)
	void WaveStateChanged(EWaveState NewState, EWaveState OldState);
};
