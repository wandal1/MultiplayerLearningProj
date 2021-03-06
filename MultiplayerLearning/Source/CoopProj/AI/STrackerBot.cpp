// Fill out your copyright notice in the Description page of Project Settings.

#include "STrackerBot.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NavigationSystem/Public/NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Components/SHealthComponent.h"
#include "Components/SphereComponent.h"
#include "Player/SCharacter.h"
#include "Sound/SoundCue.h"
#include "Engine/EngineTypes.h"

// Sets default values
ASTrackerBot::ASTrackerBot()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComp;
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);

	HealthComp = CreateDefaultSubobject<USHealthComponent>(TEXT("Health"));
	HealthComp->OnHealthChanged.AddDynamic(this, &ASTrackerBot::TakeDamageHandle);

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("ExplosionSphere"));
	SphereComp->SetSphereRadius(200);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ASTrackerBot::BeginPlay()
{
	Super::BeginPlay();

	if (Role == ROLE_Authority)
	{
		NextPathPoint = GetNextPathPoint();
	}

	MatInst = MeshComp->CreateAndSetMaterialInstanceDynamicFromMaterial(0, MeshComp->GetMaterial(0));

	GetWorldTimerManager().SetTimer(Search_TH, this, &ASTrackerBot::TryFindBotsNear, 1, true, 0);

}

void ASTrackerBot::TakeDamageHandle(class USHealthComponent* OtherHealthComp, float Health, float HealthDeltaAActor, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (MatInst)
	{
		MatInst->SetScalarParameterValue("LastTimeDamageTaken", GetWorld()->TimeSeconds);
	}

	if (Health <= 0)
	{
		SelfDestruct();
	}
}

FVector ASTrackerBot::GetNextPathPoint()
{
	APawn* PlayerPawn = nullptr;
	float NearestDistance = FLT_MAX;

	for (FConstPlayerControllerIterator It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PC = It->Get();
		if (PC && PC->GetPawn())
		{
			APawn* MyPawn = PC->GetPawn();
			if (USHealthComponent::IsFriendly(MyPawn, this))
			{
				continue;
			}

			USHealthComponent* MyPawnHealthComp = Cast<USHealthComponent>(MyPawn->GetComponentByClass(USHealthComponent::StaticClass()));

			if (ensure(HealthComp) && MyPawnHealthComp->IsAlive())
			{
				float Distance = (MyPawn->GetActorLocation() - GetActorLocation()).Size();

				if (Distance < NearestDistance)
				{
					PlayerPawn = MyPawn;
					NearestDistance = Distance;
				}
			}
		}
	}

	if (PlayerPawn)
	{
		UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), PlayerPawn);

		GetWorldTimerManager().ClearTimer(RefreshPath_TH);
		GetWorldTimerManager().SetTimer(RefreshPath_TH, this, &ASTrackerBot::RefreshPath, 5, false);

		if (NavPath && NavPath->PathPoints.Num() > 1)
		{
			return NavPath->PathPoints[1];
		}
	}

	return GetActorLocation();
}

void ASTrackerBot::SelfDestruct()
{
	if (bExploded)
		return;

	bExploded = true;

	if (ExplosionEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplosionEffect, GetActorTransform());
	}
	UGameplayStatics::SpawnSoundAttached(ExplosionSound, RootComponent);

	MeshComp->SetVisibility(false);
	MeshComp->SetSimulatePhysics(false);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (Role == ROLE_Authority)
	{
		TArray<AActor*> IgnoredActors;
		IgnoredActors.Add(this);

		UGameplayStatics::ApplyRadialDamage(this, ExplosionDamage + (BotsNear * BotsNearMultiplier), GetActorLocation(), ExplosionRadius, nullptr, IgnoredActors, this, GetInstigatorController(), true);

		SetLifeSpan(2);
	}
}

void ASTrackerBot::SelfDamage()
{
	if (!bExploded)
	{
		UGameplayStatics::ApplyDamage(this, 20, GetInstigatorController(), this, nullptr);
	}
}

void ASTrackerBot::RefreshPath()
{
	NextPathPoint = GetNextPathPoint();
}

void ASTrackerBot::TryFindBotsNear()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(EObjectTypeQuery::ObjectTypeQuery2);
	TArray<AActor*> ActorToIgnore;
	ActorToIgnore.Add(this);
	TArray<AActor*> OuterActors;

	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetActorLocation(), SearchRadius, ObjectTypes, ASTrackerBot::StaticClass(), ActorToIgnore, OuterActors);

	BotsNear = OuterActors.Num();

	if (BotsNear > 0)
	{
		if (MatInst)
		{
			MatInst->SetScalarParameterValue("BotsNear", GetWorld()->TimeSeconds);
		}
	}
}

// Called every frame
void ASTrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Role != ROLE_Authority)
		return;

	if (bExploded)
		return;

	FVector ForceDirection;

	RefreshPath();

	if (NextPathPoint.Size() > StopDistance) {
		FVector NextTracePoint = NextPathPoint;
		ForceDirection = NextPathPoint - GetActorLocation();
		float DistanceToTarget = FVector::Distance(NextTracePoint, GetActorLocation());

		if (DistanceToTarget > StopDistance) 
		{
			ForceDirection -= GetVelocity();
		}

		if (DistanceToTarget <= DistanceTolerance)
		{
			NextPathPoint = GetNextPathPoint();
		}

		ForceDirection.Z = 0.0F;
		ForceDirection.Normalize();
		MeshComp->AddForce(ForceDirection * MovementForce, NAME_None, true);
	}
	//Speed down after close to Character enough.
	else {
		if (GetVelocity().Size() <= StopVelocity) {
			ForceDirection = -GetVelocity();
			ForceDirection.Normalize();
			MeshComp->AddForce(ForceDirection * MovementForce, NAME_None, true);
		}
	}

	if (false)
	{
		FVector VelocityLineEnd = GetActorLocation() + (GetVelocity().GetSafeNormal() * 200);
		FVector TargetLineEnd = GetActorLocation() + (ForceDirection.GetSafeNormal() * 200);

		DrawDebugSphere(GetWorld(), NextPathPoint, 50, 12, FColor::Red, true, 0, 0, 5);

		DrawDebugLine(GetWorld(), GetActorLocation(), VelocityLineEnd, FColor::Red, false, 0, 0, 11);
		DrawDebugLine(GetWorld(), GetActorLocation(), TargetLineEnd, FColor::Blue, false, 0, 0, 11);
	}
}

void ASTrackerBot::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!USHealthComponent::IsFriendly(OtherActor, this) && !bExploded)
	{

		if (Role == ROLE_Authority)
		{
			GetWorldTimerManager().SetTimer(SelfDamage_TH, this, &ASTrackerBot::SelfDamage, SelfDamageInterval, true, 0);
		}

		UGameplayStatics::SpawnSoundAttached(SelfDestructionSound, RootComponent);
	}
}