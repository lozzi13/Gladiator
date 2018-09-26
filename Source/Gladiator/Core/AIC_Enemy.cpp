// Fill out your copyright notice in the Description page of Project Settings.

#include "AIC_Enemy.h"

#include "Gladiator.h"
#include "Character/Hero.h"
#include "Character/Enemy.h"

#include "BehaviorTree/BehaviorTree.h"
#include "AI/Navigation/NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

AAIC_Enemy::AAIC_Enemy()
{
	ConstructorHelpers::FObjectFinder<UBehaviorTree> TreeAsset(TEXT("/Game/Core/AI/BT_Enemy.BT_Enemy"));
	if (TreeAsset.Succeeded())
		BTree = TreeAsset.Object;
}

void AAIC_Enemy::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BTree);
	Target = Cast<AWarrior>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (!Target)
		return;

	SetFocus(Target);
	if (Blackboard)
	{
		Blackboard->SetValueAsObject("Target", Target);
		Blackboard->SetValueAsFloat("Radius", SafeDist);
		Blackboard->SetValueAsFloat("DelayAfterAttack", DelayAfterAttack);
	}
}

void AAIC_Enemy::Reset()
{
	Super::Reset();

	FTimerManager& Time { GetWorldTimerManager() };
	Time.ClearTimer(AttackTimer);
}

void AAIC_Enemy::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	Self = Cast<AWarrior>(GetPawn());
	if (!Self)
		UnPossess();
}

void AAIC_Enemy::StartAttack()
{
	if (!IsValid(Self) || !IsValid(Target))
		return;

	FVector MyLocation { Self->GetActorLocation() };
	FVector TargetLocation { Target->GetActorLocation() };

	float Dist { FVector::Dist(MyLocation, TargetLocation) };

	if (Self->CanAttack() && Dist <= DistToAttack)
		Self->Attack();

	if (DelayAfterAttack > 0.f)
	{
		FTimerManager& Time { GetWorldTimerManager() };
		Time.SetTimer(AttackTimer, this, &AAIC_Enemy::StopAttack, DelayAfterAttack, false);
	}
	else if (DelayAfterAttack == 0.f)
		StopAttack();
}

void AAIC_Enemy::StopAttack()
{
	SetShouldAttack(false);
}

void AAIC_Enemy::Stop()
{
	SetFocus(nullptr);
	StopMovement();

	if (BrainComponent)
		BrainComponent->StopLogic("Dead");
}

void AAIC_Enemy::SetAngle(float Angle)
{
	if (!Blackboard)
		return;

	Blackboard->SetValueAsFloat("Angle", Angle);
}

void AAIC_Enemy::SetShouldAttack(bool bShouldAttack)
{
	if (!Blackboard)
		return;

	Blackboard->SetValueAsBool("bShouldAttack", bShouldAttack);
}

bool AAIC_Enemy::GetShouldAttack() const
{
	if (!Blackboard)
		return false;

	return Blackboard->GetValueAsBool("bShouldAttack");
}