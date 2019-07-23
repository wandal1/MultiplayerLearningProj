// Fill out your copyright notice in the Description page of Project Settings.


#include "STimerVolume.h"
#include "Components\ShapeComponent.h"
#include "Components\DecalComponent.h"
#include "Components\BoxComponent.h"

ASTimerVolume::ASTimerVolume()
{
// 	PrimaryActorTick.bCanEverTick = true;
// 	PrimaryActorTick.TickInterval = 1;

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("Decalcomponent"));
	DecalComp->SetupAttachment(RootComponent);

	DecalComp->DecalSize = Cast<UBoxComponent>(GetCollisionComponent())->GetScaledBoxExtent();
	DecalComp->SetRelativeRotation(FRotator(90, 0, 0));
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
	GetWorldTimerManager().SetTimer(TimerTick_TH, this, &ASTimerVolume::TimerTick, 1, true);
	GetWorldTimerManager().SetTimer(TimerDuration_TH, this, &ASTimerVolume::TimerFinished, TimerDuration, true);
}

void ASTimerVolume::TimerFinished()
{
	ClearTimer();
}

void ASTimerVolume::TimerInterupted()
{
	ClearTimer();
}

void ASTimerVolume::ClearTimer()
{
	GetWorldTimerManager().ClearTimer(TimerTick_TH);
	GetWorldTimerManager().ClearTimer(TimerDuration_TH);
}

void ASTimerVolume::BeginPlay()
{
	Super::BeginPlay();

	RootComponent->SetVisibility(true);
	SetActorHiddenInGame(false);
}
