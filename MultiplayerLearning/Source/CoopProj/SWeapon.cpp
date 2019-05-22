// Fill out your copyright notice in the Description page of Project Settings.

#include "SWeapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "CoopProj.h"
#include "UnrealMathUtility.h"
#include "Vector.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

static int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef CVarDebugWeaponDrawing(
	TEXT("COOP.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw debug weapon trace"),
	ECVF_Cheat);


// Sets default values
ASWeapon::ASWeapon()
{
	MeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	RootComponent = MeshComp;

	AttachSocket = "MuzzleSocket";

	FireRate = 60 / BulletsPerMinute;

	SetReplicates(true);

	NetUpdateFrequency = 66;
	MinNetUpdateFrequency = 33;
}

void ASWeapon::Fire()
{
	if (Role < ROLE_Authority)
	{
		ServerFire();
	}

	if (!CanMakeShot())
	{
		StopFire();
		return;
	}

	//Trace from eyes to center of screen (aim)
	if (GetOwner())
	{
		FVector EyeLocation;
		FRotator EyeRot;
		GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRot);

		FVector ShotDirection = EyeRot.Vector();
		FVector FinalHitDirection = UKismetMathLibrary::RandomUnitVectorInConeInDegrees(ShotDirection, ScatterAngleRange);
		FVector TraceEnd = EyeLocation + (FinalHitDirection * 10000);

		FCollisionQueryParams QParams;
		QParams.AddIgnoredActor(GetOwner());
		QParams.AddIgnoredActor(this);
		QParams.bTraceComplex = true;
		QParams.bReturnPhysicalMaterial = true;

		FVector TracerEndPoint = TraceEnd;
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		FHitResult Hit;

		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_WEAPON, QParams))
		{
			AActor* HitActor = Hit.GetActor();

			SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());

			PlayImpactEffects(SurfaceType, Hit.ImpactPoint);

			float ActualDamage = BaseDamage;
			ActualDamage *= SurfaceType == SURFACE_BODY_CRITICAL ? CriticalMultiplier : 1;

			UGameplayStatics::ApplyPointDamage(HitActor, ActualDamage, FinalHitDirection, Hit, GetOwner()->GetInstigatorController(), this, DamageType);

			TracerEndPoint = Hit.ImpactPoint;
		}

		if (DebugWeaponDrawing > 0)
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::Blue, false, 1, 0, 1);
		}

		PlayExplosionEffects(TracerEndPoint);

		if (Role == ROLE_Authority)
		{
			HitScanTrace.TraceTo = TracerEndPoint;
			HitScanTrace.SurfaceType = SurfaceType;
		}

		LastFireTime = GetWorld()->TimeSeconds;

		CurrentClimpAmount--;
	}
}

void ASWeapon::PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint)
{
	UParticleSystem* TmpParticle;
	float ActualDamage = BaseDamage;

	switch (SurfaceType)
	{
	case SURFACE_DEFAULT:
		TmpParticle = ImpactEffect_Default;
		break;
	case SURFACE_BODY:
		TmpParticle = ImpactEffect_Body;
		break;
	case SURFACE_BODY_CRITICAL:
		TmpParticle = ImpactEffect_BodyCritical;
		break;
	default:
		TmpParticle = ImpactEffect_Default;
		break;
	}

	if (TmpParticle)
	{
		FVector MuzzleLocation = MeshComp->GetSocketLocation(AttachSocket);
		FVector ShootDirection = ImpactPoint - MuzzleLocation;
		ShootDirection.Normalize();

		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TmpParticle, ImpactPoint, ShootDirection.Rotation());
	}
}

void ASWeapon::OnRep_HitScanTrace()
{
	PlayExplosionEffects(HitScanTrace.TraceTo);
	PlayImpactEffects(HitScanTrace.SurfaceType, HitScanTrace.TraceTo);
}

void ASWeapon::ServerFire_Implementation()
{
	Fire();
}

bool ASWeapon::ServerFire_Validate()
{
	return true;
}

bool ASWeapon::CanMakeShot()
{
	if (CurrentClimpAmount > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool ASWeapon::CanMakeAutoReload()
{
	if (bAllowAutoReload)
	{
		return true;
	}
	
	return false;
}

void ASWeapon::StartFire()
{
	if (!CanMakeShot())
		return;

	float FirestDelay = FMath::Max<float>((LastFireTime + FireRate - GetWorld()->TimeSeconds), 0.0);

	GetWorld()->GetTimerManager().SetTimer(Fire_TH, this, &ASWeapon::Fire, FireRate, true, FirestDelay);
}

void ASWeapon::StopFire()
{
	GetWorld()->GetTimerManager().ClearTimer(Fire_TH);
}

void ASWeapon::PlayExplosionEffects(FVector TracerEndPoint)
{
	if (MuzzleEffect)
	{
		UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, MeshComp, AttachSocket);
	}

	if (TracerEffect)
	{
		FTransform TmpTransform;
		GetMuzzleSocketTransform(TmpTransform);
		UParticleSystemComponent* TraceParticle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, TmpTransform.GetLocation());

		if (TraceParticle)
		{
			TraceParticle->SetVectorParameter("Source", TmpTransform.GetLocation());
			TraceParticle->SetVectorParameter("Target", TracerEndPoint);

		}
	}

	APawn* TmpOwner = Cast<APawn>(GetOwner());
	if (TmpOwner)
	{
		APlayerController* PC = Cast<APlayerController>(TmpOwner->GetController());
		if (PC)
		{
			PC->ClientPlayCameraShake(FireCameraShake);
		}
	}

}

void ASWeapon::ReloadComplete()
{
	CurrentClimpAmount = ClipCapacity;
}

void ASWeapon::GetMuzzleSocketTransform(FTransform& SocketTransform)
{
	SocketTransform = MeshComp->GetSocketTransform(AttachSocket);
}

void ASWeapon::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION(ASWeapon, HitScanTrace, COND_SkipOwner);
}