// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class COOPPROJ_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	ASCharacter();

	virtual FVector GetPawnViewLocation() const override;

	float DefaultFOV;
	bool bWantToZoom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void BeginCrouch();
	void EndCrouch();

	void StartZoom();
	void EndZoom();

	void MakeJump();

	void StartFire();
	void StopFire();

	void ReloadStart();
	void ReloadFinished();

	void SpawnAndAttachWeapon();

	UFUNCTION(NetMulticast, Reliable)
		void OnHealthCahnged(class USHealthComponent* NewHealthComp, float Health, float HealthDeltaAActor, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class UCameraComponent* CameraComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USpringArmComponent* SpringArmComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		class USHealthComponent* HealthComp;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zoom")
		float ZoomFOV;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zoom", meta = (ClampMin = 0.1, ClampMax = 100))
		float ZoomInterpolationSpeed = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
		TSubclassOf<class ASWeapon> WeaponToSpawn_Class;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Zoom")
		FName WeaponSocket = "WeaponSocket";
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Alive")
		bool bIsAlive = true;

private:

	UPROPERTY(Replicated)
		class ASWeapon* CurrentWeapon;

public:
	UPROPERTY(BlueprintReadOnly)
		bool bReloadInProcess = false;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
