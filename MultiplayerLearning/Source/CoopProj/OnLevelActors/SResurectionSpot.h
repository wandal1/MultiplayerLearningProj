// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnLevelActors/STimerVolume.h"
#include "SResurectionSpot.generated.h"

/**
 * 
 */
UCLASS()
class COOPPROJ_API ASResurectionSpot : public ASTimerVolume
{
	GENERATED_BODY()
	
protected:
	virtual void TimerFinished() override;

};
