// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpsBase.h"
#include "Components/SphereComponent.h"
#include "Components/DecalComponent.h"
#include "PowerUpActor.h"

// Sets default values
APickUpsBase::APickUpsBase()
{

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	RootComponent = SphereComp;
	SphereComp->SetSphereRadius(75);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComp->SetupAttachment(RootComponent);

	DecalComp->SetRelativeRotation(FRotator(90, 0 ,0));
	DecalComp->DecalSize = FVector(64, 75, 75);

}

// Called when the game starts or when spawned
void APickUpsBase::BeginPlay()
{
	Super::BeginPlay();
	
	Respawn();
}

void APickUpsBase::Respawn()
{
	if (!PowerUpClass)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	SpawnedPowerUp = GetWorld()->SpawnActor<APowerUpActor>(PowerUpClass, GetTransform(), SpawnParams);
}

void APickUpsBase::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (SpawnedPowerUp)
	{
		SpawnedPowerUp->ActivatePowerUp();

		GetWorldTimerManager().SetTimer(Cooldomwn_TH, this, &APickUpsBase::Respawn, Cooldown + SpawnedPowerUp->PowerUpsInterval);
		
		SpawnedPowerUp = nullptr;

	}
}

