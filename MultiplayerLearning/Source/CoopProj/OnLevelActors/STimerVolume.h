// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "STimerVolume.generated.h"

/**
 * 
 */
UCLASS()
class COOPPROJ_API ASTimerVolume : public ATriggerBox
{
	GENERATED_BODY()
	
public:
	ASTimerVolume();

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;


	virtual void OnConstruction(const FTransform& Transform) override;


	virtual void Tick(float DeltaSeconds) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UDecalComponent* DecalComp;

	UPROPERTY(EditAnywhere, Category = "Timer")
		float TimerDuration = 10;

	UPROPERTY()
	FTimerHandle TimerTick_TH;
	UPROPERTY()
	FTimerHandle TimerDuration_TH;


	UFUNCTION(BlueprintImplementableEvent)
		void TimerTick();

	bool TryStartTimer();

	bool IsTimerRuning();

	void StartTimer();

	UFUNCTION()
	virtual void TimerFinished();

	void TimerInterupted();

	void ClearTimer();

	virtual void BeginPlay() override;
};
