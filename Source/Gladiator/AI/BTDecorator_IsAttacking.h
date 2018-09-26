// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_IsAttacking.generated.h"

UCLASS()
class GLADIATOR_API UBTDecorator_IsAttacking : public UBTDecorator
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector Target;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	public:
	UBTDecorator_IsAttacking();
};
