// Fill out your copyright notice in the Description page of Project Settings.

#include "SGranadeLauncher.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/World.h"
#include "SLauncherProjectile.h"

ASGranadeLauncher::ASGranadeLauncher()
{
}

void ASGranadeLauncher::BeginPlay()
{
	Super::BeginPlay();
}

void ASGranadeLauncher::Fire()
{
	if (!ProjectileClass)
		return;

	if (!GetOwner())
		return;

	FVector EyeLocation;
	FRotator EyeRot;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRot);

	FTransform TmpTransform;
	GetMuzzleSocketTransform(TmpTransform);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	ASLauncherProjectile* TmpProjectile = GetWorld()->SpawnActor<ASLauncherProjectile>(ProjectileClass, TmpTransform.GetLocation(), EyeRot, SpawnParams);
}
