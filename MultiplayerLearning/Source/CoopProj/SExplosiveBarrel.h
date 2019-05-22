// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SExplosiveBarrel.generated.h"

UCLASS()
class COOPPROJ_API ASExplosiveBarrel : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASExplosiveBarrel();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		class UStaticMeshComponent* MeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		class USHealthComponent* HealthComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Component")
		class URadialForceComponent* RadialForceComp;
	UFUNCTION()
		void OnHealthCahnged(class USHealthComponent* BarrelHealthComp, float Health, float HealthDeltaAActor, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void ApplyExplosionEffects();


	UPROPERTY(EditDefaultsOnly, Category = "FX")
		float ExplosionImpulse = 500;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		class UParticleSystem* ExplosionEffect;
	UPROPERTY(EditDefaultsOnly, Category = "FX")
		class UMaterialInterface* ExplosionMaterial;
	UPROPERTY(Replicated, ReplicatedUsing = OnRep_Exploded)
		bool bExploded = false;
	
	UFUNCTION()
		void OnRep_Exploded();

};
