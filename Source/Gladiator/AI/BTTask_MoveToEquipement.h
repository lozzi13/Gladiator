// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_MoveToEquipement.generated.h"

struct FBTMoveToEquipementTaskMemory
{
	FAIRequestID MoveRequestID;
};

UCLASS()
class GLADIATOR_API UBTTask_MoveToEquipement : public UBTTaskNode
{
	GENERATED_BODY()
	
	protected:
	/*	Blackboard keys	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector EquipementClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector Equipement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FBlackboardKeySelector MaxDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float AcceptableDistance;

	/*	Functions	*/
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message,	int32 RequestID, bool bSuccess) override;
	virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

	virtual class AEquipement* FindClosestEquipement(const TSubclassOf<class AEquipement>& ActorClass, 
													 const AActor* Self, float MaxDist) const;
	
	virtual FPathFollowingRequestResult SetupMove(class AAIC_Enemy* Controller, class UBlackboardComponent* Blackboard);
	
	public:
	UBTTask_MoveToEquipement();
};
