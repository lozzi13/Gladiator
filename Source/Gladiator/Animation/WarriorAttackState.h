// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "WarriorAttackState.generated.h"

UCLASS()
class GLADIATOR_API UWarriorAttackState : public UAnimNotifyState
{
	GENERATED_BODY()
	
	public:
	virtual FString GetNotifyName_Implementation() const override;

	virtual void NotifyBegin(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(class USkeletalMeshComponent* MeshComp, class UAnimSequenceBase* Animation) override;
};
