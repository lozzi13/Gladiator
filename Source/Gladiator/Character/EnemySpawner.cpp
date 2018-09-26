// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemySpawner.h"

#include "Gladiator.h"
#include "Core/GladiatorGameModeBase.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	EnemyCount = BaseEnemyCount;
	CurrentTime = 0.f;

	// Notify Game Mode
	AGameModeBase* Mode { UGameplayStatics::GetGameMode(GetWorld()) };
	AGladiatorGameModeBase* GameMode { Cast<AGladiatorGameModeBase>(Mode) };
	if (!GameMode)
		return;

	GameMode->NotifyEnemyCount(BaseEnemyCount);
}

void AEnemySpawner::Reset()
{
	Super::Reset();

	EnemyCount = BaseEnemyCount;
	PrimaryActorTick.bCanEverTick = EnemyCount >= 0;
	CurrentTime = 0.f;

	// Notify Game Mode
	AGameModeBase* Mode { UGameplayStatics::GetGameMode(GetWorld()) };
	AGladiatorGameModeBase* GameMode { Cast<AGladiatorGameModeBase>(Mode) };
	if (!GameMode)
		return;

	GameMode->NotifyEnemyCount(BaseEnemyCount);
}

void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (EnemyCount <= 0)
	{
		PrimaryActorTick.bCanEverTick = false;
		return;
	}

	CurrentTime += DeltaTime;
	if (CurrentTime < SpawnDelay)
		return;

	Spawn();
	CurrentTime = 0.f;
}

void AEnemySpawner::Spawn()
{
	// Spawn an enemy
	UWorld* World { GetWorld() };
	if (!World)
		return;

	FVector Location { GetActorLocation() };
	FRotator Rotation { GetActorRotation() };

	World->SpawnActor(EnemyClass, &Location, &Rotation);

	// Decrease Spawner stock
	--EnemyCount;
}

