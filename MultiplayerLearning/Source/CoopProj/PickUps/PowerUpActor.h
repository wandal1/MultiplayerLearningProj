// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PowerUpActor.generated.h"

UCLASS()
class COOPPROJ_API APowerUpActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APowerUpActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
		int32 TotalNOfTicks = 0;

	int32 TickMade = 0;

	FTimerHandle PowerupTick_TH;

	UFUNCTION()
		void PowerUpTick();

public:

	UPROPERTY(EditDefaultsOnly, Category = "PowerUps")
		float PowerUpsInterval = 0;

	void ActivatePowerUp();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
		void OnActivated();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
		void OnPowerUpTicked();

	UFUNCTION(BlueprintImplementableEvent, Category = "PowerUps")
		void OnExpired();

};
