// Fill out your copyright notice in the Description page of Project Settings.


#include "STimerVolume.h"
#include "Components\ShapeComponent.h"
#include "Components\DecalComponent.h"
#include "Components\BoxComponent.h"

ASTimerVolume::ASTimerVolume()
{
// 	PrimaryActorTick.bCanEverTick = true;
// 	PrimaryActorTick.TickInterval = 1;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decalcomponent"));
	DecalComp->SetupAttachment(RootComponent);

	DecalComp->DecalSize = Cast<UBoxComponent>(GetCollisionComponent())->GetScaledBoxExtent();
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));

	SetReplicates(true);
}

void ASTimerVolume::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	TryStartTimer();
}

void ASTimerVolume::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	TArray<AActor*> Actors;
	GetCollisionComponent()->GetOverlappingActors(Actors);

	if (Actors.Num() < 1)
	{
		TimerInterupted();
	}
}

void ASTimerVolume::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	DecalComp->DecalSize = Cast<UBoxComponent>(GetCollisionComponent())->GetScaledBoxExtent();
}

void ASTimerVolume::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UE_LOG(LogTemp, Warning, TEXT("Tick Timer : %s"), GetWorldTimerManager().IsTimerActive(TimerTick_TH) ? TEXT("Valid") : TEXT("Not Valid"));
	UE_LOG(LogTemp, Warning, TEXT("Direation Timer : %s"), GetWorldTimerManager().IsTimerActive(TimerDuration_TH) ? TEXT("Valid") : TEXT("Not Valid"));
}

bool ASTimerVolume::TryStartTimer()
{
	if (IsTimerRuning())
	{
		return false;
	}
	
	StartTimer();
	return true;
}

bool ASTimerVolume::IsTimerRuning()
{
	return GetWorldTimerManager().IsTimerActive(TimerDuration_TH) || GetWorldTimerManager().IsTimerActive(TimerTick_TH);
}

void ASTimerVolume::StartTimer()
{
	if (IsTimerRuning())
	{
		return;
	}

	GetWorldTimerManager().SetTimer(TimerTick_TH, this, &ASTimerVolume::TimerTick, 1, true, 1);
	GetWorldTimerManager().SetTimer(TimerDuration_TH, this, &ASTimerVolume::TimerFinished, TimerDuration, false);
}

void ASTimerVolume::TimerFinished()
{
	ClearTimer();

	UE_LOG(LogTemp, Warning, TEXT("Timer Finished"));
}

void ASTimerVolume::TimerInterupted()
{
	ClearTimer();

	UE_LOG(LogTemp, Warning, TEXT("Timer Interupted"));
}

void ASTimerVolume::ClearTimer()
{
	GetWorldTimerManager().ClearTimer(TimerTick_TH);

	GetWorldTimerManager().ClearTimer(TimerDuration_TH);

	UE_LOG(LogTemp, Warning, TEXT("Timers Cleared"));
}

void ASTimerVolume::BeginPlay()
{
	Super::BeginPlay();

	RootComponent->SetVisibility(true);
	SetActorHiddenInGame(false);
}
