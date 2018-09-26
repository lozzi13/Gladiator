// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_IsAttacking.h"

#include "Gladiator.h"
#include "Character/Warrior.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTDecorator_IsAttacking::UBTDecorator_IsAttacking()
{
	NodeName = "Is Attacking";
}

bool UBTDecorator_IsAttacking::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard { OwnerComp.GetBlackboardComponent() };
	if (!Blackboard)
		return false;

	bool bIsAttacking { Blackboard->GetValueAsBool(IsAttacking.SelectedKeyName) };
	if (!bIsAttacking)
		return false;

	AWarrior* Warrior { Cast<AWarrior>(Blackboard->GetValueAsObject(Target.SelectedKeyName)) };
	return IsValid(Warrior) && !Warrior->IsDead();
}