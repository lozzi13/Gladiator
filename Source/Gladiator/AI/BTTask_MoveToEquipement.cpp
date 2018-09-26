// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveToEquipement.h"

#include "Gladiator.h"

//#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

#include "Props/Equipement.h"
#include "Character/Warrior.h"
#include "Core/AIC_Enemy.h"

#include "BehaviorTree/BlackboardComponent.h"

UBTTask_MoveToEquipement::UBTTask_MoveToEquipement()
{
	NodeName = "Move To Equipement";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToEquipement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AAIC_Enemy* SelfController { Cast<AAIC_Enemy>(OwnerComp.GetAIOwner()) };
	UBlackboardComponent* Blackboard { OwnerComp.GetBlackboardComponent() };
	FBTMoveToEquipementTaskMemory* Memory { reinterpret_cast<FBTMoveToEquipementTaskMemory*>(NodeMemory) };
	
	if (!SelfController || !Blackboard || !Memory)
		return EBTNodeResult::Aborted;

	FPathFollowingRequestResult SetupResult { SetupMove(SelfController, Blackboard) };
	if (SetupResult.Code == EPathFollowingRequestResult::Failed)
		return EBTNodeResult::Failed;

	if (SetupResult.Code == EPathFollowingRequestResult::AlreadyAtGoal)
		return EBTNodeResult::Succeeded;

	Memory->MoveRequestID = SetupResult.MoveId;
	WaitForMessage(OwnerComp, UBrainComponent::AIMessage_MoveFinished, SetupResult.MoveId);
	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToEquipement::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
	FBTMoveToEquipementTaskMemory* Memory { reinterpret_cast<FBTMoveToEquipementTaskMemory*>(NodeMemory) };
	if (!Memory || Memory->MoveRequestID.GetID() != RequestID)
		return;

	if (Message == UBrainComponent::AIMessage_RepathFailed)
		FinishLatentAbort(OwnerComp);

	else
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
}

void UBTTask_MoveToEquipement::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIC_Enemy* SelfController { Cast<AAIC_Enemy>(OwnerComp.GetAIOwner()) };
	UBlackboardComponent* Blackboard { OwnerComp.GetBlackboardComponent() };
	if (!SelfController || !Blackboard)
	{
		FinishLatentAbort(OwnerComp);
		return;
	}

	AEquipement* FoundEquip { Cast<AEquipement>(Blackboard->GetValueAsObject(Equipement.SelectedKeyName)) };
	if (!IsValid(FoundEquip) || FoundEquip->IsHeld())
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
}

void UBTTask_MoveToEquipement::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult)
{
	AAIController* SelfController { OwnerComp.GetAIOwner() };
	if (SelfController)
		SelfController->StopMovement();

	StopWaitingForMessages(OwnerComp);
}

FPathFollowingRequestResult UBTTask_MoveToEquipement::SetupMove(AAIC_Enemy* Controller, UBlackboardComponent* Blackboard)
{
	FPathFollowingRequestResult FailedResult {};
	FailedResult.Code = EPathFollowingRequestResult::Failed;

	// Check itself
	AWarrior* Self { Cast<AWarrior>(Controller->GetPawn()) };
	if (Self->IsAttacking())
		return FailedResult;

	// Find Equipement
	UClass* ActorClass { Blackboard->GetValueAsClass(EquipementClass.SelectedKeyName) };
	float MaxDist { Blackboard->GetValueAsFloat(MaxDistance.SelectedKeyName) };

	AEquipement* FoundEquip { FindClosestEquipement(ActorClass, Self, MaxDist) };
	if (!FoundEquip)
		return FailedResult;

	Blackboard->SetValueAsObject(Equipement.SelectedKeyName, FoundEquip);

	// Go to Equipement
	Controller->SetFocus(FoundEquip);

	FAIMoveRequest MoveRequest { FoundEquip };
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAllowPartialPath(true);
	MoveRequest.SetNavigationFilter(Controller->GetDefaultNavigationFilterClass());
	MoveRequest.SetAcceptanceRadius(AcceptableDistance);
	MoveRequest.SetReachTestIncludesAgentRadius(true);
	MoveRequest.SetCanStrafe(true);

	return Controller->MoveTo(MoveRequest);
}

AEquipement* UBTTask_MoveToEquipement::FindClosestEquipement(const TSubclassOf<AEquipement>& ActorClass, const AActor* Self, float MaxDist) const
{
	UWorld* World { GetWorld() };
	if (!IsValid(World) || !IsValid(Self))
		return nullptr;

	// Get all equipements of the given class
	TArray<AActor*> FoundActors {};
	UGameplayStatics::GetAllActorsOfClass(World, ActorClass, FoundActors);

	if (FoundActors.Num() <= 0)
		return nullptr;

	// Sort by distance
	FoundActors.Sort([Self](const AActor& A, const AActor& B)
	{
		float ADist { FVector::DistSquared(A.GetActorLocation(), Self->GetActorLocation()) };
		float BDist { FVector::DistSquared(B.GetActorLocation(), Self->GetActorLocation()) };

		return ADist < BDist;
	});

	// Find nearest not already held
	AEquipement* Nearest { nullptr };
	for (int i = 0 ; i < FoundActors.Num() && !Nearest ; ++i)
	{
		AEquipement* Current { Cast<AEquipement>(FoundActors[i]) };
		if (!IsValid(Current) || Current->IsHeld())
			continue;

		Nearest = Current;
	}

	return Nearest;
}