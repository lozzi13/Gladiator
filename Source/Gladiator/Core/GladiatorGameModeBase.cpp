// Fill out your copyright notice in the Description page of Project Settings.

#include "GladiatorGameModeBase.h"

#include "Gladiator.h"
#include "AI/AIDirector.h"
#include "Character/Warrior.h"

#include "EngineUtils.h"
#include "Engine/LevelScriptActor.h"

AGladiatorGameModeBase::AGladiatorGameModeBase()
{
	EnemyCount = 0;
	bEnded = false;
	AIDirector = CreateDefaultSubobject<UAIDirector>(TEXT("AIDirector"));
}


void AGladiatorGameModeBase::Win_Implementation()
{
	OnWin.Broadcast();
}

void AGladiatorGameModeBase::Lose_Implementation()
{
	OnLose.Broadcast();
}

void AGladiatorGameModeBase::Restart()
{}

void AGladiatorGameModeBase::Quit()
{
	FGenericPlatformMisc::RequestExit(false);
}

void AGladiatorGameModeBase::NotifyEnemyCount(int CountToAdd)
{
	if (CountToAdd <= 0)
		return;

	EnemyCount += CountToAdd;
}

int AGladiatorGameModeBase::GetNbEnemy()
{
	return EnemyCount;
}

void AGladiatorGameModeBase::NotifyEnemySpawned(AWarrior* Enemy)
{
	if (AIDirector)
		AIDirector->AddEnemy(Enemy);
}

void AGladiatorGameModeBase::NotifyEnemyKilled(AWarrior* Enemy)
{
	if (AIDirector)
		AIDirector->RemoveEnemy(Enemy);

	if (EnemyCount == 0)
		return;

	if (--EnemyCount <= 0 && !bEnded)
	{
		Win();
		bEnded = true;
	}
}

void AGladiatorGameModeBase::NotifyPlayerKilled()
{
	if (!bEnded)
	{
		Lose();
		bEnded = true;
	}
}

void AGladiatorGameModeBase::Reset()
{
	Super::Reset();

	EnemyCount = 0;
	bEnded = false;
	AIDirector->Reset();
}

void AGladiatorGameModeBase::ResetLevel()
{
	UWorld* World{ GetWorld() };
	if (!World)
		return;

	/*	Inherited from GameModeBase	*/
	UE_LOG(LogGameMode, Verbose, TEXT("Reset %s"), *GetName());

	// Reset the GameMode
	Reset();

	// Reset ALL controllers first
	APlayerController* FirstController{ nullptr };
	for (FConstControllerIterator It = World->GetControllerIterator(); It; ++It)
	{
		AController* Controller = It->Get();
		APlayerController* PlayerController{ Cast<APlayerController>(Controller) };
		if (PlayerController)
		{
			PlayerController->ClientReset();

			if (!FirstController)
				FirstController = PlayerController;
		}

		Controller->Reset();
	}

	// Reset all actors
	// Except controllers, the GameMode, actors specified by ShouldReset() and destroy warriors
	for (FActorIterator It{ World }; It; ++It)
	{
		AActor* A = *It;
		if (!A || A->IsPendingKill() || A == this || A->IsA<AController>())
			continue;

		if (ShouldReset(A))
			A->Reset();

		// Whether it's an Enemy or the Player
		if (A->IsA<AWarrior>())
			A->Destroy();
	}

	// Respawn the player
	if (FirstController)
		RestartPlayer(FirstController);

	// Reset Level
	ALevelScriptActor* LevelScript{ World->GetLevelScriptActor() };
	if (LevelScript)
		LevelScript->LevelReset();
}


UAIDirector* AGladiatorGameModeBase::GetAIDirector()
{
	return AIDirector;
}

void AGladiatorGameModeBase::CanAttack()
{
	AIDirector->SetIsHostile(true);
}