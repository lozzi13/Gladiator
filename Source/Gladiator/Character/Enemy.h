// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Warrior.h"
#include "Enemy.generated.h"

UCLASS()
class GLADIATOR_API AEnemy : public AWarrior
{
	GENERATED_BODY()

	protected:
	virtual void OnDeath_Implementation() override;
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bIsLocked;

	UFUNCTION(BlueprintImplementableEvent)
	void OnLock();

	public:
	AEnemy();

	virtual void SetLocked(bool Val);
};

