// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

#include "Gladiator.h"
#include "Core/GladiatorGameModeBase.h"
#include "Core/AIC_Enemy.h"

#include "GameFramework/GameModeBase.h"
#include "Components/CapsuleComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	bIsLocked = false;

	UCapsuleComponent* Capsule { GetCapsuleComponent() };
	Capsule->SetCollisionObjectType(ECollisionChannel::ECC_ENEMY);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	bIsLocked = false;
	AGameModeBase* Mode{ UGameplayStatics::GetGameMode(GetWorld()) };
	AGladiatorGameModeBase* GameMode{ Cast<AGladiatorGameModeBase>(Mode) };
	if (!GameMode)
		return;
	
	GameMode->NotifyEnemySpawned(this);
}

void AEnemy::OnDeath_Implementation()
{
	Super::OnDeath_Implementation();

	UCapsuleComponent* Capsule { GetCapsuleComponent() };
	if (IsValid(Capsule))
		Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	AGameModeBase* Mode { UGameplayStatics::GetGameMode(GetWorld()) };
	AGladiatorGameModeBase* GameMode { Cast<AGladiatorGameModeBase>(Mode) };
	if (!GameMode)
		return;

	bIsLocked = false;
	
	AAIC_Enemy* EnemyController { Cast<AAIC_Enemy>(Controller) };
	if (EnemyController)
		EnemyController->Stop();

	GameMode->NotifyEnemyKilled(this);
}

void AEnemy::SetLocked(bool Val)
{
	bIsLocked = Val;
	OnLock();
}