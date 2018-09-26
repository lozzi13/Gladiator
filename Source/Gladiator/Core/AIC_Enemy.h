// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "AIC_Enemy.generated.h"

UCLASS()
class GLADIATOR_API AAIC_Enemy : public AAIController
{
	GENERATED_BODY()

	protected:
	class UBehaviorTree* BTree;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SafeDist;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistToAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DelayAfterAttack;

	FTimerHandle AttackTimer;

	class AWarrior* Self;
	class AWarrior* Target;

	virtual void BeginPlay() override;
	virtual void Reset() override;
	virtual void Possess(APawn* InPawn) override;

	public:
	AAIC_Enemy();

	UFUNCTION(BlueprintCallable)
	virtual void StartAttack();

	UFUNCTION(BlueprintCallable)
	virtual void StopAttack();

	virtual void Stop();
	
	virtual void SetAngle(float Angle);
	virtual void SetShouldAttack(bool bShouldAttack);

	virtual bool GetShouldAttack() const;
};
