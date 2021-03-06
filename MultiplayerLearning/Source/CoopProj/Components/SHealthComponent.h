// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_SixParams(FOnHealthChangedSignature, USHealthComponent*, HealthComp, float, Health, float, HealthDeltaAActor, const class UDamageType*, DamageType, class AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS(/*ClassGroup = (COOP),*/ /*meta = (BlueprintSpawnableComponent)*/)
class COOPPROJ_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USHealthComponent();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Team")
	uint8 TeamNum = 255;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(ReplicatedUsing = OnRep_Health, BlueprintReadOnly)
		float CurrentHealth;

	UFUNCTION()
		void OnRep_Health(float Oldhealth);

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Health")
		float MaxHealth = 100;

	bool bIsAlive = true;

	UFUNCTION()
		void HandleAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

public:

	UFUNCTION(BlueprintCallable)
	bool IsAlive() const;

	UFUNCTION(BlueprintCallable)
	float GetHealth() const;

	/*Get Health amount in range from 0 to 1*/
	UFUNCTION(BlueprintCallable)
		float GetHealthPercentage() const;

	UPROPERTY(BlueprintAssignable)
		FOnHealthChangedSignature OnHealthChanged;

	UFUNCTION(BlueprintCallable)
		void Heal(float HealtAmount);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsFriendly(AActor* ActorA, AActor* ActorB);
};
