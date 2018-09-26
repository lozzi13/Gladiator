// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_MoveToSafePlace.generated.h"

UCLASS()
class GLADIATOR_API UBTTask_MoveToSafePlace : public UBTTaskNode
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Radius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Angle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f))
	float TooCloseRadius;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FVector GetFarDestination(FVector SelfLocation, FVector TargetLocation, UBlackboardComponent* Blackboard) const;
	virtual FVector GetSafeDestination(FVector SelfLocation, FVector TargetLocation, UBlackboardComponent* Blackboard) const;

	public:
	UBTTask_MoveToSafePlace();
};
