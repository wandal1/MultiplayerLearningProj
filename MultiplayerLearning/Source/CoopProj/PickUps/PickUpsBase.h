// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PickUpsBase.generated.h"

UCLASS()
class COOPPROJ_API APickUpsBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpsBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		class USphereComponent* SphereComp;
	UPROPERTY(VisibleAnywhere)
		class UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		TSubclassOf<class APowerUpActor> PowerUpClass;
	UPROPERTY(EditDefaultsOnly, Category = "PowerUp")
		float Cooldown = 1;

	FTimerHandle Cooldomwn_TH;
	
	void Respawn();

	UPROPERTY()
	APowerUpActor* SpawnedPowerUp;

public:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

};
