// Fill out your copyright notice in the Description page of Project Settings.

#include "AIDirector.h"

#include "Gladiator.h"
#include "Core/GladiatorGameModeBase.h"
#include "Core/AIC_Enemy.h"

#include "Character/Enemy.h"

#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"

UAIDirector::UAIDirector()
{
	NbEnemy = 0;
	AICharacters.SetNum(0);
	bIsHostile = false;
}

void UAIDirector::BeginPlay()
{
	Super::BeginPlay();

	if (AttackDelay <= 0.f)
		AttackDelay = 1.f;
}

void UAIDirector::Reset()
{
	NbEnemy = 0;
	CurrentAttacker = nullptr;
	AICharacters.SetNum(0);

	SetIsHostile(true);
}

void UAIDirector::AddEnemy(AWarrior* Actor)
{
	if (!IsValid(Actor))
		return;

	AICharacters.SetNum(NbEnemy + 1);
	AICharacters[NbEnemy++] = Actor;
	
	RecalculateAngles();
}

void UAIDirector::RemoveEnemy(AWarrior* Actor)
{
	if (!IsValid(Actor))
		return;

	AICharacters.Remove(Actor);

	if (NbEnemy > 0)
		AICharacters.SetNum(--NbEnemy);

	RecalculateAngles();
}

void UAIDirector::RecalculateAngles()
{
	if (NbEnemy <= 0)
		return;

	float Step = TotalAngleRot / NbEnemy;

	for (int i = 0; i < NbEnemy; i++)
	{
		AWarrior* Chara { AICharacters[i] };
		if (!IsValid(Chara))
			continue;

		AAIC_Enemy* EnemyController = Cast<AAIC_Enemy>(Chara->GetController());
		if (!EnemyController)
			continue;

		EnemyController->SetAngle(Step * i + Step);
	}
}

void UAIDirector::OrderAttack()
{
	// Can't attack if no enemy
	if (NbEnemy <= 0)
		return;

	// Can't attack if an enemy is attacking
	if (IsValid(CurrentAttacker) && CurrentAttacker->IsAttacking())
		return;
	
	// Check if at least one enemy can attack
	bool bNoAttacker { true };
	for (int i = 0 ; i < NbEnemy && bNoAttacker ; ++i)
		bNoAttacker = IsValid(AICharacters[i]) ? !AICharacters[i]->CanAttack() : true;

	if (bNoAttacker)
		return;

	// Find a random enemy who can attack
	CurrentAttacker = nullptr;
	while (!IsValid(CurrentAttacker) || !CurrentAttacker->CanAttack())
	{
		int IndexChoosenAI { FMath::RandRange(0, NbEnemy - 1) };
		CurrentAttacker = AICharacters[IndexChoosenAI];
	}
	
	// Set the enemy value so he attacks
	AAIC_Enemy* EnemyController = Cast<AAIC_Enemy>(CurrentAttacker->GetController());
	if (!EnemyController)
		return;
	
	EnemyController->SetShouldAttack(true);
}

void UAIDirector::LockEnemy(int Index)
{
	UnlockAll();

	if (Index >= NbEnemy || Index < 0)
		return;

	AEnemy* Enemy = Cast<AEnemy>(AICharacters[Index]);
	if (!Enemy)
		return;

	Enemy->SetLocked(true);
}

void UAIDirector::UnlockAll()
{
	for (int i = 0 ; i < NbEnemy; i++)
	{
		AEnemy* Enemy = Cast<AEnemy>(AICharacters[i]);
		if (!Enemy)
			return;

		Enemy->SetLocked(false);
	}
}

AWarrior* UAIDirector::GetEnemy(int Index)
{
	if (NbEnemy <= 0)
		return nullptr;

	int ChosenIndex { abs(Index % NbEnemy) };
	LockEnemy(ChosenIndex);

	return AICharacters[ChosenIndex];
}

void UAIDirector::SetIsHostile(bool bHostile)
{
	if (bIsHostile == bHostile)
		return;

	bIsHostile = bHostile;

	UWorld* World { GetWorld() };
	if (!World)
		return;

	FTimerManager& TimerMgr { World->GetTimerManager() };
	if (bIsHostile)
		TimerMgr.SetTimer(TimeHandle, this, &UAIDirector::OrderAttack, AttackDelay, true);

	else
		TimerMgr.ClearTimer(TimeHandle);
}