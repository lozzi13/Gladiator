// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToSafePlace.h"

#include "Gladiator.h"
#include "Core/AIC_Enemy.h"
#include "Character/Warrior.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToSafePlace::UBTTask_MoveToSafePlace()
{
	NodeName = "Move To Safe Place";
}

EBTNodeResult::Type UBTTask_MoveToSafePlace::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Check itself
	AAIC_Enemy* SelfController { Cast<AAIC_Enemy>(OwnerComp.GetAIOwner()) };
	UBlackboardComponent* Blackboard { OwnerComp.GetBlackboardComponent() };
	if (!SelfController || !Blackboard)
		return EBTNodeResult::Aborted;

	AWarrior* Self { Cast<AWarrior>(SelfController->GetPawn()) };
	if (Self->IsAttacking())
		return EBTNodeResult::Failed;

	// Check Target
	AWarrior* Warrior { Cast<AWarrior>(Blackboard->GetValueAsObject(Target.SelectedKeyName)) };
	if (!IsValid(Warrior) || Warrior->IsDead())
		return EBTNodeResult::Aborted;

	FVector TargetLocation = Warrior->GetActorLocation();
	FVector SelfLocation = Self->GetActorLocation();
	
	// Check distance & calculate destination
	float Distance { FVector::DistSquaredXY(TargetLocation, SelfLocation) };
	FVector Destination {};
	if (Distance > TooCloseRadius * TooCloseRadius)
		Destination = GetSafeDestination(SelfLocation, TargetLocation, Blackboard);
	else
		Destination = GetFarDestination(SelfLocation, TargetLocation, Blackboard);

	// Move
	EPathFollowingRequestResult::Type MoveResult { SelfController->MoveToLocation(Destination) };
	if (MoveResult == EPathFollowingRequestResult::Failed)
		return EBTNodeResult::Failed;

	return EBTNodeResult::Succeeded;
}
FVector UBTTask_MoveToSafePlace::GetFarDestination(FVector SelfLocation, FVector TargetLocation, UBlackboardComponent* Blackboard) const
{
	if (!Blackboard)
		return SelfLocation;

	// Get parameters
	float SafeDist { Blackboard->GetValueAsFloat(Radius.SelectedKeyName) };

	// Destination
	FVector ToSelf { SelfLocation - TargetLocation };
	ToSelf.Normalize();

	return ToSelf * SafeDist + TargetLocation;
}

FVector UBTTask_MoveToSafePlace::GetSafeDestination(FVector SelfLocation, FVector TargetLocation, UBlackboardComponent* Blackboard) const
{
	if (!Blackboard)
		return SelfLocation;

	// Get parameters
	float SafeDist { Blackboard->GetValueAsFloat(Radius.SelectedKeyName) };
	float AngleToTarget { Blackboard->GetValueAsFloat(Angle.SelectedKeyName) };

	// Convert to degrees
	AngleToTarget *= PI / 180.f;

	// Destination
	FVector ToSelf = SafeDist * FVector { cosf(AngleToTarget), sinf(AngleToTarget), 0.f };
	return ToSelf + TargetLocation;
}
