// Fill out your copyright notice in the Description page of Project Settings.

#include "SHealthComponent.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "UnrealMathUtility.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	CurrentHealth = MaxHealth;

	SetIsReplicated(true);
}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	if (GetOwnerRole() == ROLE_Authority)
	{
		AActor* CurrentOwner = GetOwner();
		if (CurrentOwner)
		{
			CurrentOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::HandleAnyDamage);
		}
	}
}


void USHealthComponent::OnRep_Health(float Oldhealth)
{
	OnHealthChanged.Broadcast(this, CurrentHealth, Oldhealth - CurrentHealth, nullptr, nullptr, nullptr);
}

void USHealthComponent::HandleAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Damage <= 0 || !IsAlive())
	{
		return;
	}

	CurrentHealth = FMath::Max<float>(CurrentHealth - Damage, 0);

	if (CurrentHealth <= 0)
	{
		bIsAlive = false;
	}

	UE_LOG(LogTemp, Log, TEXT("Current Health : %s"), *FString::SanitizeFloat(CurrentHealth));

	OnHealthChanged.Broadcast(this, CurrentHealth, Damage, DamageType, InstigatedBy, DamageCauser);
}

bool USHealthComponent::IsAlive() const
{
	return bIsAlive;
}

float USHealthComponent::GetHealth() const
{
	return CurrentHealth;
}

void USHealthComponent::Heal(float HealthAmount)
{
	if (HealthAmount <= 0 || CurrentHealth <= 0)
	{
		return;
	}

	CurrentHealth = FMath::Clamp<float>(CurrentHealth + HealthAmount, 0, MaxHealth);

	UE_LOG(LogTemp, Log, TEXT("Health Amount : %s"), *FString::SanitizeFloat(HealthAmount));

	OnHealthChanged.Broadcast(this, CurrentHealth, -HealthAmount, nullptr, nullptr, nullptr);
}

void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, CurrentHealth);
}

