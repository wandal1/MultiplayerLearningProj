// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "STrackerBot.generated.h"

UCLASS()
class COOPPROJ_API ASTrackerBot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASTrackerBot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleDefaultsOnly)
		class UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleDefaultsOnly)
		class USHealthComponent* HealthComp;
	UPROPERTY(VisibleDefaultsOnly)
		class USphereComponent* SphereComp;

	UPROPERTY(EditDefaultsOnly)
		float MovementForce = 1000;
	UPROPERTY(EditDefaultsOnly)
		float DistanceTolerance = 20;
	UPROPERTY(EditDefaultsOnly)
		float StopDistance = 200.0F;
	UPROPERTY(EditDefaultsOnly)
		float StopVelocity = 10.0F;
	UPROPERTY(EditDefaultsOnly)
		float ExplosionDamage = 100.0F;
	UPROPERTY(EditDefaultsOnly)
		float BotsNearMultiplier = 10.0F;
	UPROPERTY(EditDefaultsOnly)
		float ExplosionRadius = 200.0F;
	UPROPERTY(EditDefaultsOnly)
		float SearchRadius = 300.0F;
	UPROPERTY(EditDefaultsOnly)
		class UParticleSystem* ExplosionEffect;
	UPROPERTY(EditDefaultsOnly)
		bool bUseVelocityChange = false;
	UPROPERTY(EditDefaultsOnly)
		class USoundCue* ExplosionSound;
	UPROPERTY(EditDefaultsOnly)
		class USoundCue* SelfDestructionSound;
	UPROPERTY(EditDefaultsOnly)
		float SelfDamageInterval = 0.25;

	UPROPERTY()
		FVector NextPathPoint;
	bool bExploded = false;
	int BotsNear = 0;

	UMaterialInstanceDynamic* MatInst;
	FTimerHandle SelfDamage_TH;
	FTimerHandle Search_TH;
	FTimerHandle RefreshPath_TH;

	UFUNCTION()
		void TakeDamageHandle(class USHealthComponent* NewHealthComp, float Health, float HealthDeltaAActor, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	FVector GetNextPathPoint();
	void SelfDestruct();
	void SelfDamage();

	void RefreshPath();
	void TryFindBotsNear();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
