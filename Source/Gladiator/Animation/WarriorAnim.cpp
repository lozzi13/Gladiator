// Fill out your copyright notice in the Description page of Project Settings.

#include "WarriorAnim.h"

#include "Gladiator.h"

#include "Character/Warrior.h"
#include "Core/PC_Hero.h"

#include "WarriorAttackState.h"

void UWarriorAnim::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	APawn* Pawn { TryGetPawnOwner() };
	Warrior = Cast<AWarrior>(Pawn);
	if (!IsValid(Warrior))
		return;
}

void UWarriorAnim::NotifyAttack(bool bAttacking)
{
	if (!IsValid(Warrior))
		return;

	Warrior->OnNotifyAttack(bAttacking);
}

void UWarriorAnim::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!IsValid(Warrior))
		return;

	bIsDead = Warrior->IsDead();
	bHitting = Warrior->IsAttacking();

	if (bIsDead)
	{
		Speed = FVector2D { 0.f, 0.f };
		return;
	}

	FRotator Rotation { Warrior->GetActorRotation() };
	FVector Velocity { Warrior->GetVelocity() };

	Speed = FVector2D { Rotation.UnrotateVector(Velocity) };

	APC_Hero* HeroController { Cast<APC_Hero>(Warrior->GetController()) };
	bCanStrafe = !IsValid(HeroController) || HeroController->IsLocking();
}