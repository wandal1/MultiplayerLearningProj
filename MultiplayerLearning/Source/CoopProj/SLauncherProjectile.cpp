// Fill out your copyright notice in the Description page of Project Settings.

#include "SLauncherProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "Particles/ParticleSystemComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ASLauncherProjectile::ASLauncherProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjMovement->bShouldBounce = true;

	SM_Bullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
	SM_Bullet->SetupAttachment(RootComponent);

	SM_Bullet->SetCollisionProfileName("BlockAll");
	//SM_Bullet->BodyInstance.bNotifyRigidBodyCollision = true;

}

void ASLauncherProjectile::LaunchProjectile(FVector Direction)
{
	if (!ProjMovement)
		return;

	ProjMovement->SetVelocityInLocalSpace(Direction * InitialSpeed);
}

// Called when the game starts or when spawned
void ASLauncherProjectile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TmpHandle;
	GetWorld()->GetTimerManager().SetTimer(TmpHandle, this, &ASLauncherProjectile::MakeExplosion, TimeBeforeExplosion, false);

	//SM_Bullet->OnComponentHit.AddDynamic(this, &ASLauncherProjectile::HandleBlock);
}

void ASLauncherProjectile::MakeExplosion()
{
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}

	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), GetActorRotation());
	}

	UGameplayStatics::ApplyRadialDamage(this, 20, GetActorLocation(), 250, NULL, { this});

	Destroy();
}

// Called every frame
void ASLauncherProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

