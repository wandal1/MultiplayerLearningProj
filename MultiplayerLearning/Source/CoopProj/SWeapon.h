// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

// Info for hit scan line trace
USTRUCT()
struct FHitScanTrace
{

	GENERATED_BODY()

public:
	UPROPERTY()
		TEnumAsByte<EPhysicalSurface> SurfaceType;

	UPROPERTY()
		FVector_NetQuantize TraceTo;

};

UCLASS()
class COOPPROJ_API ASWeapon : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASWeapon();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Mesh")
		class USkeletalMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		TSubclassOf<class UDamageType> DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float BaseDamage = 20;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float CriticalMultiplier = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float BulletsPerMinute = 600;
	//Will randomize scatter in range from -ScatterAngleRange to ScatterAngleRange 
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float ScatterAngleRange = 0.5;


	//Delay between fire
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem* MuzzleEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem* ImpactEffect_Default;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem* ImpactEffect_Body;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem* ImpactEffect_BodyCritical;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class TSubclassOf<class UCameraShake> FireCameraShake;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		FName AttachSocket;
	UPROPERTY(EditDefaultsOnly, Category = "Reload")
		bool bAllowAutoReload = true;
	UPROPERTY(EditDefaultsOnly, Category = "Reload")
		int ClipCapacity = 30;

	UPROPERTY(ReplicatedUsing = OnRep_HitScanTrace)
		FHitScanTrace HitScanTrace;

	float LastFireTime;
	int CurrentClimpAmount = ClipCapacity;

	void GetMuzzleSocketTransform(FTransform& SocketTransform);

	void PlayExplosionEffects(FVector TracerEndPoint);

	virtual void Fire();

	void PlayImpactEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);

	UFUNCTION(Server, Reliable, WithValidation)
		void ServerFire();

	UFUNCTION()
		void OnRep_HitScanTrace();
public:

	UPROPERTY(EditDefaultsOnly, Category = "Reload")
		class UAnimMontage* ReloadMontage;

	bool CanMakeShot();
	bool CanMakeAutoReload();
	void ReloadComplete();

	FTimerHandle Fire_TH;

	void StartFire();

	void StopFire();
};
