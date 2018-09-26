// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTDecorator.h"
#include "BTDecorator_NeedEquipement.generated.h"

UCLASS()
class GLADIATOR_API UBTDecorator_NeedEquipement : public UBTDecorator
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FBlackboardKeySelector EquipementClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = 0.f, ClampMax = 1.f))
	float LifeThreshold;

	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	public:
	UBTDecorator_NeedEquipement();
};
