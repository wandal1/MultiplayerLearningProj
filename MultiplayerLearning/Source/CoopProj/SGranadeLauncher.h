// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SWeapon.h"
#include "SGranadeLauncher.generated.h"

/**
 * 
 */
UCLASS()
class COOPPROJ_API ASGranadeLauncher : public ASWeapon
{
	GENERATED_BODY()
	

public:

	ASGranadeLauncher();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ASLauncherProjectile> ProjectileClass;

protected:
	virtual void BeginPlay() override;

	virtual void Fire() override;

};
