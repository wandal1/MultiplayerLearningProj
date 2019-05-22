// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SLauncherProjectile.generated.h"

UCLASS()
class COOPPROJ_API ASLauncherProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLauncherProjectile();

	void LaunchProjectile(FVector Direction);

	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float InitialSpeed = 1000;
	UPROPERTY(EditDefaultsOnly, Category = "Speed")
		float TimeBeforeExplosion = 2;

	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class UParticleSystem* ExplosionEffect;
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
		class USoundBase* ExplosionSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

//		UFUNCTION()
//		void HandleBlock(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	UFUNCTION()
		void MakeExplosion();


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UProjectileMovementComponent* ProjMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		class UStaticMeshComponent* SM_Bullet;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
