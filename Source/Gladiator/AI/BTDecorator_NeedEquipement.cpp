// Fill out your copyright notice in the Description page of Project Settings.

#include "BTDecorator_NeedEquipement.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

#include "Character/Warrior.h"
#include "Props/Shield.h"
#include "Props/Weapon.h"

UBTDecorator_NeedEquipement::UBTDecorator_NeedEquipement()
{
	NodeName = "Need Equipement";
}

bool UBTDecorator_NeedEquipement::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* Blackboard { OwnerComp.GetBlackboardComponent() };
	AAIController* Controller { OwnerComp.GetAIOwner() };
	if (!IsValid(Controller) || !Blackboard)
		return false;

	AWarrior* Self { Cast<AWarrior>(Controller->GetPawn()) };
	if (!IsValid(Self) || Self->IsDead())
		return false;

	if (!IsValid(Self->GetShield()))
	{
		float LifePercent { Self->GetLife() / Self->GetMaxLife() };
		if (LifePercent <= LifeThreshold)
		{
			Blackboard->SetValueAsClass(EquipementClass.SelectedKeyName, AShield::StaticClass());
			return true;
		}
	}

	if (!IsValid(Self->GetWeapon()))
	{
		Blackboard->SetValueAsClass(EquipementClass.SelectedKeyName, AWeapon::StaticClass());
		return true;
	}

	return false;
}